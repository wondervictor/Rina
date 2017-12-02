# -*- coding: utf-8 -*-

import socket
import log
import json
import time
import os
import sys
import common
from message import Message
from collections import deque


class ClientSocket(object):

    def __init__(self):
        self.logger = log.Logger('ClientSocket')
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.logger.info("Client Init")

    def conn(self, server_ip, port):
        self.socket.connect((server_ip, port))

    def send_message(self, buf):
        self.socket.send(buf)
        print('Send: %s' % buf)
        self.logger.info('Send: %s' % buf)

    def recv_message(self, size):
        msg = self.socket.recv(size)
        return msg

    def stop(self):
        self.socket.close()


class Client(object):

    def __init__(self):
        self.logger = log.Logger('Client')
        self.socket = ClientSocket()
        self.username = ''

    def login(self, server, port, username):
        self.socket.conn(server, port)
        self.logger.info('Conn Success')
        self.username = username
        while True:

            input_str = raw_input('>>>:')

            if input_str == 'login':
                message = Message(username, 'LOGIN', '0.0.0.0', int(time.time()))
                self.socket.send_message(message.generate())

                recv_msgs = self.socket.recv_message(1024)
                print(recv_msgs)
                recv_msgs = Message.decode(recv_msgs)
                msg = recv_msgs[0]
                if msg.type() == 'SUCCESS':
                    self.logger.info('Login Success')
                else:
                    self.logger.warn('Login Failed')
                    exit()
                continue
            if input_str == 'logout':
                message = Message(username, 'LOGOUT', '0.0.0.0', int(time.time()))
                self.socket.send_message(message.generate())
                recv_msgs = self.socket.recv_message(1024)
                recv_msgs = Message.decode(recv_msgs)
                msg = recv_msgs[0]
                if msg.type() == 'SUCCESS':
                    self.logger.info('Logout Success')
                else:
                    self.logger.warn('Logout Failed')
                    exit()
                self.logout()
                break
            if input_str == 'getall':
                message = Message(username, 'GET_MESSAGES', '0.0.0.0', int(time.time()))
                self.socket.send_message(message.generate())
                recv_msgs = self.socket.recv_message(1024)
                print(recv_msgs)
                recv_msgs = Message.decode(recv_msgs)
                self.handle_messages(recv_msgs)
                continue

            message = Message(username, 'NORMAL', '0.0.0.0', int(time.time()), input_str)
            self.socket.send_message(message.generate())

    def handle_messages(self, msgs):

        for msg in msgs:
            print('[%s] %s (%s): %s' % (common.time_to_str(msg.get_timestamp()), msg.get_username(), msg.get_addr(),
                                        msg.get_content()))

    def logout(self):

        pass


def test():

    client = Client()
    client.login('0.0.0.0', int(sys.argv[1]), sys.argv[2])




test()