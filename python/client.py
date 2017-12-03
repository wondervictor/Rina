# -*- coding: utf-8 -*-

import socket
import log
import json
import time
import os
import sys
import common
import thread
from message import Message
import Queue

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
        self.message_queue = Queue.Queue()

    def login_start(self, server, port, username, callback=None):
        """
        :param server: server ip address
        :param port: server port
        :param username: username
        :param callback: callback when messages arrived def callback(message)
        :return:
        """
        self.socket.conn(server, port)
        self.logger.info('Conn Success')
        self.username = username
        while True:

            input_str = raw_input('>>>:')

            if input_str == 'login':

                def callback_func(recv_msgs):
                    msg = recv_msgs[0]
                    if msg.type() == 'SUCCESS':
                        self.logger.info('Login Success')
                    else:
                        self.logger.warn('Login Failed')
                        exit()

                message = Message(username, 'LOGIN', '0.0.0.0', int(time.time()))
                self.send_recv(message, callback_func)

                if callback:
                    callback(message)
                continue

            if input_str == 'logout':
                message = Message(username, 'LOGOUT', '0.0.0.0', int(time.time()))

                def callback_func(recv_msgs):
                    msg = recv_msgs[0]
                    if msg.type() == 'SUCCESS':
                        self.logger.info('Logout Success')
                    else:
                        self.logger.warn('Logout Failed')
                        exit()
                    self.logout()
                self.send_recv(message, callback_func)
                if callback:
                    callback(message)
                break

            if input_str == 'getall':
                message = Message(username, 'GET_MESSAGES', '0.0.0.0', int(time.time()))
                self.send_recv(message, self.handle_messages)

                if callback:
                    callback(message)
                continue

            if input_str == 'getuser':
                message = Message(username, 'GET_USERS', '0.0.0.0', int(time.time()))

                def callback_func(message):
                    message = message[0]
                    self.handle_users(message.get_content())

                self.send_recv(message, callback_func)

                if callback:
                    callback(message)
                continue

            message = Message(username, 'NORMAL', '0.0.0.0', int(time.time()), input_str)
            self.socket.send_message(message.generate())

    def request(self, message, callback=None):
        self.socket.send_message(message.generate())
        recv_msgs = self.socket.recv_message(1024)
        self.logger.info('[Client]Recv: %s' % recv_msgs)
        recv_msgs = Message.decode(recv_msgs)
        if callback:
            callback(recv_msgs)

    def send_recv(self, message, callback):
        self.message_queue.put((message, callback))

    def run_queue(self):
        while True:
            if not self.message_queue.empty():
                print("Queue.items: %s" % self.message_queue.qsize())
                (message, callback) = self.message_queue.get()
                self.request(message, callback)

    def start(self):

        thread.start_new_thread(self.run_queue, ())

    def login(self, username, callback):
        message = Message(username, 'LOGIN', '0.0.0.0', int(time.time()))

    def update_message(self, callback):
        message = Message(self.username, 'GET_MESSAGES', '0.0.0.0', int(time.time()))

    def update_users(self, callback):
        message = Message(self.username, 'GET_USERS', '0.0.0.0', int(time.time()))

    def handle_users(self, userstr):

        users = userstr.split(',')
        print("User List:")
        for us in users:
            print(us)

    def handle_messages(self, msgs):

        for msg in msgs:
            print('[%s] %s (%s): %s' % (common.time_to_str(msg.get_timestamp()), msg.get_username(), msg.get_addr(),
                                        msg.get_content()))

    def logout(self):

        self.logger.info("Logout")


def test():

    client = Client()
    client.start()
    client.login_start('0.0.0.0', int(sys.argv[1]), sys.argv[2])




test()