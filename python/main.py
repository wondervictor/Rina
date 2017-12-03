# -*- coding: utf-8 -*-


"""
Content: Main View
Author: Vic Chan
Date: 2017/10/19
"""


from Tkinter import *
import tkMessageBox
from client import Client
from message import Message


class LoginView(Frame):

    def __init__(self, app, master=None,):
        Frame.__init__(self, master, width=200, height=200)
        self.app = app
        self.pack()
        self.user_label = Label(self, text="User:")
        self.user_label.grid(row=0, column=0, sticky=W)
        self.user_entry = Entry(self)
        self.user_entry.grid(row=0, column=1, sticky=W)

        self.server_ip = Label(self, text="IP:")
        self.server_ip.grid(row=1, column=0, sticky=W)

        self.server_entry = Entry(self)
        self.server_entry.grid(row=1, column=1, sticky=W)

        self.login_button = Button(self, text="Login", command=self.login, height=5, width=20)
        self.login_button.grid(row=2, column=0, sticky=W)

        self.cancel_button = Button(self, text="Cancel", command=self.cancel)
        self.cancel_button.grid(row=3, column=0, sticky=W)

    def login(self):
        if not isinstance(self.app, Application):
            exit(-1)
        username = self.user_entry.get()
        ip = self.server_entry.get()
        ip, port = ip.split(':')
        self.app.login(username, ip, int(port))

    def cancel(self):
        exit(0)


class Application(object):

    def __init__(self):
        self.client = Client()
        self.root = Tk()
        self.root.wm_attributes('-topmost', 1)

    def _resize_window(self, width, height):
        screenwidth = self.root.winfo_screenwidth()
        screenheight = self.root.winfo_screenheight()
        size = '%dx%d+%d+%d' % (width, height, (screenwidth - width) / 2, (screenheight - height) / 2)
        self.root.geometry(size)

    def login(self, username, ip, port):

        def login_callback(message):
            msg = message[0]
            if msg.type() == 'SUCCESS':
                tkMessageBox.showinfo("提示信息", "登录成功")
            else:
                tkMessageBox.showinfo("提示信息", "登录失败")
                exit()

        self.client.start(
            server=ip,
            port=port,
            username=username
        )

        self.client.login(
            username,
            callback=login_callback
        )

    def start(self):

        login = LoginView(self, self.root)
        self._resize_window(400, 200)
        self.root.mainloop()


def test():

    app = Application()
    app.start()


test()
