# -*- coding: utf-8 -*-


import socket
import log
import thread
import time
import sys
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
        self.messages = []
        self.logger = log.Logger('Server')

    def name(self):
        return "SERVER"

    def _handle(self, conn, addr):

        def event(server, _conn, _addr):

            if not isinstance(server, Server):
                self.logger.error("server is not the RIGHT instance")
                exit(-1)

            while True:
                msg = _conn.recv(buf_size)
                if len(msg) == 0:
                    continue
                msg = Message.decode(msg)[0]
                self.logger.info("[Server]Recv: %s" % msg)
                if not isinstance(msg, Message):
                    self.logger.warn("[Server]message error")
                    continue
                msg_type = msg.type()

                if msg_type == 'LOGIN':
                    username = msg.get_username()
                    new_user = User(
                        name=username,
                        conn=conn,
                        state=UserState.Online,
                        update=0
                    )
                    server.login(username, new_user)
                    self.logger.info("[Server]user %s joined" % username)
                    new_msg = Message(
                        name=server.name(),
                        type='SUCCESS',
                        addr=server.addr,
                        timestamp=get_timestamp()
                    )
                    msg_str = new_msg.generate()
                    conn.send(msg_str)

                elif msg_type == 'LOGOUT':
                    username = msg.get_username()
                    server.logout(username)
                    self.logger.info("[Server]user %s leaved" % username)
                    new_msg = Message(
                        name=server.name(),
                        type='SUCCESS',
                        addr=server.addr,
                        timestamp=get_timestamp()
                    )
                    msg_str = new_msg.generate()
                    conn.send(msg_str)
                    return

                elif msg_type == 'GET_MESSAGES':
                    username = msg.get_username()
                    messages = server.get_messages(username)
                    self.logger.info("[Server]send to %s count: %s" % (username, len(messages)))
                    msg_str = Message.generate_many(messages)
                    conn.send(msg_str)

                elif msg_type == 'GET_USERS':

                    username = msg.get_username()
                    self.logger.info("[Server]User: %s Get Users" % username)
                    new_msg = Message(
                        name=server.name(),
                        type='NORMAL',
                        addr=server.addr,
                        timestamp=get_timestamp(),
                        content=server.get_users()
                    )
                    msg_str = new_msg.generate()
                    conn.send(msg_str)

                else:
                    username = msg.get_username()
                    self.logger.info("[Server] user %s send: %s" % (username, msg.get_content()))
                    server.add_message(msg)
                    new_msg = Message(
                        name=server.name(),
                        type='SUCCESS',
                        addr=server.addr,
                        timestamp=get_timestamp()
                    )
                    msg_str = new_msg.generate()
                    conn.send(msg_str)

        thread.start_new_thread(event, (self, conn, addr))
        self.logger.info("[Server] Create a new thread")

    def start(self):

        self.logger.info("Server is running ...")

        self.socket.listen(20)
        while True:

            conn, addr = self.socket.accept()
            self._handle(conn, addr)

    def login(self, username, user):
        self.users[username] = user

    def logout(self, username):
        if username in self.users:
            del self.users[username]

    def add_message(self, message):

        self.messages.append(message)

    def get_messages(self, username):

        user = self.users[username]
        idx = user.update
        messages = self.messages[idx:]
        self.users[username].update_seq(len(messages))
        return messages

    def get_users(self):

        user_str = ','.join(self.users.keys())
        return user_str


def test():

    server = Server(int(sys.argv[1]))
    server.start()

test()






