import abc
import logging
import logging.handlers
import pickle
import signal
import socketserver
import struct
import subprocess
import time

import riaps.log.visualizers.tmux as visualizer


class BaseLogHandler(socketserver.StreamRequestHandler):

    def __init__(self, request, client_address, server):
        self.view = server.view
        self.logger = logging.getLogger(__name__)
        super(BaseLogHandler, self).__init__(request, client_address, server)

    def setup(self) -> None:
        super(BaseLogHandler, self).setup()

    @abc.abstractmethod
    def handle(self) -> None:
        pass

    def handle_log_record(self, data):
        self.logger.info(f"client address: {self.client_address}")
        node_name = self.client_address[0]
        if node_name not in self.view.nodes:
            self.view.add_node_display(node_name=node_name)
        self.view.write_display(node_name=node_name, msg=data)


class AppLogHandler(BaseLogHandler):

    def handle(self) -> None:
        while True:
            data_bytes = self.rfile.readline().strip()
            if not data_bytes:
                break
            data_string = data_bytes.decode("utf-8")
            data = f"{data_string}"
            self.handle_log_record(data)


class PlatformLogHandler(BaseLogHandler):

    def handle(self) -> None:
        while True:
            chunk = self.connection.recv(4)
            if len(chunk) < 4:
                break
            slen = struct.unpack('>L', chunk)[0]
            chunk = self.connection.recv(slen)
            while len(chunk) < slen:
                chunk += self.connection.recv(slen - len(chunk))
            obj = pickle.loads(chunk)
            obj["msg"] = f"{obj['msg']}: {self.client_address}"
            record = logging.makeLogRecord(obj)
            self.handle_log_record(data=record.getMessage())


class BaseLogServer(socketserver.ThreadingTCPServer):

    def __init__(self, server_address, RequestHandlerClass, view):
        # logger.basicConfig(level=logging.INFO)
        self.allow_reuse_address = True
        self.logger = logging.getLogger(__name__)
        self.RequestHandlerClass = RequestHandlerClass
        self.view = view
        super(BaseLogServer, self).__init__(server_address,
                                            RequestHandlerClass)

    # def finish_request(self, request, client_address):
    #     node_name = client_address[0]
    #     try:
    #         if node_name not in self.view.nodes:
    #             self.logger.warning(f"\nclient_address: {client_address}"
    #                                 f"\nNEW NODE: {node_name} "
    #                                 f"\nALL NODES: {self.view.nodes.keys()}")
    #
    #             self.view.add_node_display(node_name=node_name)
    #     except subprocess.CalledProcessError as e:
    #         self.logger.error(f"Was the Tmux session closed?\n"
    #                           f" {e}")
    #
    #     self.RequestHandlerClass(request, client_address, self)

    def serve_until_stopped(self):
        self.logger.info(f'About to start Log server {self.view.session_name}...')
        # Activate the server; this will keep running until you
        # interrupt the program with Ctrl-C
        # self.serve_forever()
        self.serve_forever()

    # def server_close(self):
    #     # self.logger.info("Send shutdown to stop handling requests")
    #     # self.shutdown()
    #     # self.logger.info("Server is shutdown")
    #     # super(BaseLogServer, self).server_close()
    #     # self.logger.info("Log server closed")
    #     self.view.close_session()



if __name__ == '__main__':
    import multiprocessing
    view = visualizer.View(session_name="platform")
    theLogServer = BaseLogServer(server_address=("172.21.20.70",
                                 logging.handlers.DEFAULT_TCP_LOGGING_PORT),
                                 RequestHandlerClass=PlatformLogHandler,
                                 view=view)

    logger = logging.getLogger(__name__)
    server = multiprocessing.Process(target=theLogServer.serve_until_stopped)
    server.start()

    def term_handler(signal, frame):
        print("Call term_handler")
        server.terminate()
        view.close_session()

    signal.signal(signal.SIGTERM, term_handler)
    signal.signal(signal.SIGINT, term_handler)


