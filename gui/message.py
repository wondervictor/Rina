# -*- coding: utf-8 -*-

import json


class Message(object):

    def __init__(self, name, type, addr, timestamp, content=None):
        self._name = name
        self._type = type
        self._addr = addr
        self._content = content
        self._timestamp = timestamp

    def _get_type_str(self, type):

        if type == 'LOGIN':
            return '*2c411521vn148-e=1r'
        if type == 'LOGOUT':
            return 'e21874vnwv1o2870810'
        if type == 'GET_ALL':
            return '1321c32-23@(*^&*#Bv'
        if type == 'LOGIN_SUCCESS':
            return '&*TBX*GR*&@BC&GF&*@'
        return self._content

    @staticmethod
    def get_type(content):
        if content == '*2c411521vn148-e=1r':
            return 'LOGIN'
        if content == 'e21874vnwv1o2870810':
            return 'LOGOUT'
        if content == '1321c32-23@(*^&*#Bv':
            return 'GET_ALL'
        if content == '&*TBX*GR*&@BC&GF&*@':
            return 'LOGIN_SUCCESS'
        return 'NORMAL'

    def type(self):
        return Message.get_type(self._content)

    def get_username(self):
        return self._name

    def get_content(self):
        return self._content

    def get_timestamp(self):
        return self._timestamp

    def get_addr(self):
        return self._addr

    def generate(self):

        msg = dict()
        msg['name'] = self._name
        msg['ip'] = self._addr
        type_content = self._get_type_str(self._type)
        msg['content'] = type_content
        msg['timestamp'] = self._timestamp

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

    def __str__(self):

        return "<Message Name: %s Content: %s Addr: %s Time: %s Type: %s>" % \
               (self._name, self._content, self._addr, self._timestamp, self.get_type(self._content))