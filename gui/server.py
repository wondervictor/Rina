# -*- coding: utf-8 -*-


import socket
import logging
import thread
import time
from message import Message
from user import User, UserState
from common import get_timestamp


buf_size = 1024


class Server(object):

    def __init__(self, port, ip='0.0.0.0'):
        self.socket = socket.socket()
        self.socket.bind((ip, port))
        self.users = dict()
        self.addr = ip

    def name(self):
        return "SERVER"

    def _handle(self, conn, addr):

        def event(server, _conn, _addr):

            if not isinstance(server, Server):
                logging.error("server is not the RIGHT instance")
                exit(-1)

            while True:
                msg = _conn.recv(buf_size)
                if len(msg) == 0:
                    continue
                msg = Message.decode(msg)[0]
                logging.info("Server Recv: %s" % msg)
                if not isinstance(msg, Message):
                    logging.warning("Server: message error")
                    continue
                msg_type = msg.type()

                if msg_type == 'LOGIN':
                    username = msg.get_username()
                    new_user = User(
                        username,
                        conn,
                        UserState.Online,
                        0
                    )
                    server.add_user(new_user)
                    logging.info("Server: user %s joined" % username)
                    new_msg = Message(
                        name=server.name(),
                        type='LOGIN_SUCCESS',
                        addr=server.addr,
                        timestamp=get_timestamp()
                    )
                    msg_str = new_msg.generate()
                    conn.send(msg_str)

                elif msg_type == 'LOGOUT':


                elif msg_type == 'GET_ALL':



                else:
                    continue





    def start(self):

        logging.info("Server is running ...")

        self.socket.listen(20)
        while True:

            conn, addr = self.socket.accept()


    def add_user(self, user):

        self.users[user.name] = user

    def add_message(self, message):





