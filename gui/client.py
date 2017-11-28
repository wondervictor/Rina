# -*- coding: utf-8 -*-

import socket
import logging
import json
import time
import os
import sys


class Message(object):

    def __init__(self, name, type, addr, timestamp, content=None):
        self.name = name
        self.type = type
        self.addr = addr
        self.content = content
        self.timestamp = timestamp

    def _get_type_str(self, type):

        if type == 'LOGIN':
            return '*2c411521vn148-e=1r'
        if type == 'LOGOUT':
            return 'e21874vnwv1o2870810'
        if type == 'GET_ALL':
            return '1321c32-23@(*^&*#Bv'
        if type == 'LOGIN_SUCCESS':
            return '&*TBX*GR*&@BC&GF&*@'
        return self.content

    @staticmethod
    def get_type(content):
        if type == '*2c411521vn148-e=1r':
            return 'LOGIN'
        if type == 'e21874vnwv1o2870810':
            return 'LOGOUT'
        if type == '1321c32-23@(*^&*#Bv':
            return 'GET_ALL'
        if type == '&*TBX*GR*&@BC&GF&*@':
            return 'LOGIN_SUCCESS'
        return 'NORMAL'

    def generate(self):

        msg = dict()
        msg['name'] = self.name
        msg['ip'] = self.addr
        type_content = self._get_type_str(self.type)
        msg['content'] = type_content
        msg['timestamp'] = self.timestamp

        msgs = [msg]

        json_enc = json.JSONEncoder()
        json_str = json_enc.encode(msgs)
        return json_str

    @staticmethod
    def decode(msg):
        decoder = json.JSONDecoder()
        msgs = decoder.decode(msg)
        messages = []
        for msg in msgs:
            name = msg['name']
            ip = msg['ip']
            content = msg['content']
            timestamp = msg['timestamp']
            messages.append(Message(name, Message.get_type(content), ip, timestamp, content))

        return messages


class ClientSocket(object):

    def __init__(self):

        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        logging.info("Client Init")

    def conn(self, server_ip, port):
        self.socket.connect((server_ip, port))

    def send_message(self, buf):
        self.socket.send(buf)
        logging.info('Send: %s' % buf)

    def recv_message(self, size):
        msg = self.socket.recv(size)
        return msg

    def stop(self):
        self.socket.close()


class Client(object):

    def __init__(self):
        self.socket = ClientSocket()
        self.username = ''

    def login(self, server, port, username):
        self.socket.conn(server, port)
        logging.info('Conn Success')
        self.username = username
        while True:

            input_str = raw_input('>>>:')

            if input_str == 'login':
                message = Message(username, 'LOGIN', '0.0.0.0', int(time.time()))
                self.socket.send_message(message.generate())
                recv_msgs = self.socket.recv_message(1024)
                recv_msgs = Message.decode(recv_msgs)
                msg = recv_msgs[0]
                if Message.get_type(msg.content) == 'LOGIN_SUCCESS':
                    logging.info('Login Success')
                else:
                    logging.warning('Login Failed')
                    exit()
                continue
            if input_str == 'logout':
                message = Message(username, 'LOGOUT', '0.0.0.0', int(time.time()))
                self.socket.send_message(message.generate())
                self.logout()
                break
            if input_str == 'getall':
                message = Message(username, 'GET_ALL', '0.0.0.0', int(time.time()))
                self.socket.send_message(message.generate())
                recv_msgs = self.socket.recv_message(1024)
                recv_msgs = Message.decode(recv_msgs)
                self.handle_messages(recv_msgs)
                continue

            message = Message(username, 'NORMAL', '0.0.0.0', int(time.time()), input_str)
            self.socket.send_message(message.generate())

    def handle_messages(self, msgs):

        for msg in msgs:
            print('[%s] %s (%s): %s' % (msg.timestamp, msg.name, msg.addr, msg.content))

    def logout(self):

        pass


def test():

    client = Client()
    client.login('0.0.0.0', 12124, 'vic')




test()