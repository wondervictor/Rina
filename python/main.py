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
from log import Logger
import time


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


class ChatView(Frame):

    def __init__(self, app, master=None):
        Frame.__init__(self, master)
        self.app = app

        self.frame_left_top = Frame(width=380, height=270, bg='white')
        self.frame_left_center = Frame(width=380, height=100, bg='white')
        self.frame_left_bottom = Frame(width=380, height=20)
        self.frame_right = Frame(width=170, height=400, bg='white')

        self.text_msglist = Text(self.frame_left_top)
        self.text_msg = Text(self.frame_left_center)
        self.button_sendmsg = Button(self.frame_left_bottom, text='发送', command=self.send_message)
        self.text_msglist.tag_config('green', foreground='#008B00')
        self.frame_left_top.grid(row=0, column=0, padx=2, pady=5)
        self.frame_left_center.grid(row=1, column=0, padx=2, pady=5)
        self.frame_left_bottom.grid(row=2, column=0)
        self.frame_right.grid(row=0, column=1, rowspan=3, padx=4, pady=5)
        self.frame_left_top.grid_propagate(0)
        self.frame_left_center.grid_propagate(0)
        self.frame_left_bottom.grid_propagate(0)
        self.text_msglist.grid()
        self.text_msg.grid()
        self.button_sendmsg.grid(sticky=E)

    def send_message(self):

        content = self.text_msg.get("1.0", END)
        self.app.logger.info("content: %s" % content)
        if not isinstance(self.app, Application):
            exit(-1)

        def send_callback(message):
            if message[0].type() != 'SUCCESS':
                self.app.logger.error("Send Failed")
            msgcontent = '我:' + time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()) + '\n  '
            self.text_msglist.insert(END, msgcontent, 'green')
            self.text_msglist.insert(END, self.text_msg.get('0.0', END))
            self.text_msg.delete('0.0', END)
            self.app.logger.info("Send Success")

        self.app.send(content, send_callback)


class Application(object):

    def __init__(self):
        self.client = Client()
        self.root = Tk()
        self.root.wm_attributes('-topmost', 1)
        self.username = ''
        self.logger = Logger('Application')
        self.current_view = None

    def _resize_window(self, width, height):
        screenwidth = self.root.winfo_screenwidth()
        screenheight = self.root.winfo_screenheight()
        size = '%dx%d+%d+%d' % (width, height, (screenwidth - width) / 2, (screenheight - height) / 2)
        self.root.geometry(size)

    def login(self, username, ip, port):

        self.username = username

        def login_callback(message):
            msg = message[0]
            if msg.type() == 'SUCCESS':
                tkMessageBox.showinfo("提示信息", "登录成功")
                self.open_chat()
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

        self.current_view = LoginView(self, self.root)
        self._resize_window(400, 200)
        self.root.mainloop()

    def send(self, msg_str, callback):

        self.client.send_message(msg_str, callback)

    def open_chat(self):
        self.current_view.destroy()
        self._resize_window(800, 400)
        self.current_view = ChatView(self)


def test():

    app = Application()
    app.start()


test()
