# -*- coding: utf-8 -*-


"""
Content: Main View
Author: Vic Chan
Date: 2017/10/19
"""


from Tkinter import *
from PIL import Image, ImageTk
import tkMessageBox
from client import Client
from message import Message
from log import Logger
import time
import threading
import thread
import sys
from ScrolledText import ScrolledText


class LoginView(Frame):

    def __init__(self, app, master=None,):
        Frame.__init__(self, master, width=300, height=200)
        self.app = app
        self.pack()

        self.center_view = Frame(self, width=100, height=100)
        self.left_frame = Frame(self, width=130, height=100)
        self.right_frame = Frame(self, width=140, height=100)

        self.user_label = Label(self.left_frame, text="用户名:",)
        self.server_ip = Label(self.left_frame, text="服务器:",)
        self.user_entry = Entry(self.right_frame, width=14)

        self.server_entry = Entry(self.right_frame, width=14)
        self.login_button = Button(self.center_view, text="登录", command=self.login, height=2, width=13)
        self.cancel_button = Button(self.center_view, text="取消", command=self.cancel, height=2, width=13)
        self.left_frame.propagate(0)
        self.right_frame.propagate(0)
        self.left_frame.grid(padx=2, pady=2, row=0, column=0)
        self.right_frame.grid(padx=0, pady=2,row=0, column=1)
        self.center_view.grid(padx=50, row=1, columnspan=2)
        self.user_label.grid(row=0)
        self.user_entry.grid(row=0)
        self.server_ip.grid(row=1, pady=7)
        self.server_entry.grid(row=1, pady=7)
        self.login_button.grid(row=0, pady=4)
        self.cancel_button.grid(row=1, pady=0)




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
        Frame.__init__(self, master, width=420, height=425, bg='gray')
        self.main_frame = Frame(width=420, height=425, bg='gray')
        self.main_frame.pack()
        self.app = app
        # message
        self.frame_left_top = Frame(self.main_frame, width=300, height=270, bg='white')
        # send message
        self.frame_left_center = Frame(self.main_frame, width=300, height=100, bg='white')
        # button
        self.frame_left_bottom = Frame(self.main_frame, width=300, height=30, bg='white')
        # users
        self.scroll_frame = Frame(self.main_frame, width=2, height=270, bg='white')

        self.frame_right = Frame(self.main_frame, width=100, height=270, bg='white')
        self.frame_right_down = Frame(self.main_frame, width=100, height=130, bg='white')
        self.sb = Scrollbar(self.scroll_frame, orient=VERTICAL)
        self.text_msglist = ScrolledText(self.frame_left_top)
        self.text_msglist['yscrollcommand'] = self.sb.set
        self.sb['command'] = self.text_msglist.yview
        self.text_msg = Text(self.frame_left_center)
        self.user_list = Text(self.frame_right, state='normal')

        self.button_sendmsg = Button(self.frame_left_bottom, bg="#00BFFF", fg="white",text='发送', command=self.send_message)
        self.button_logout = Button(self.frame_left_bottom, bg="#00BFFF", fg="white",text='退出', command=self.logout)
        self.text_msglist.tag_config('green', foreground='#008B00')
        photo = ImageTk.PhotoImage(Image.open('avatar.gif'))
        self.avatar_view = Label(self.frame_right_down, image=photo)
        self.avatar_view.image = photo
        self.frame_left_top.grid(row=0, column=0, padx=2, pady=3)
        self.frame_left_center.grid(row=1, padx=2, pady=3)
        self.frame_left_bottom.grid(row=2, column=0)
        self.scroll_frame.grid(row=0, column=1, padx=0)
        self.frame_right.grid(row=0, column=2, padx=2)
        self.frame_right_down.grid(row=1, column=2, padx=2, rowspan=2, pady=3)
        self.frame_left_top.grid_propagate(0)
        self.frame_left_center.grid_propagate(0)
        self.frame_left_bottom.grid_propagate(0)
        self.frame_right.grid_propagate(0)
        self.frame_right_down.grid_propagate(0)
        self.text_msglist.grid(row=0, column=0, sticky = E+W+S+N)
        self.text_msg.grid()
        self.user_list.grid()
        self.avatar_view.grid()
        self.sb.grid(row=0, column=1, sticky = E+W+S+N)
        self.button_sendmsg.grid(row=2, column=0)
        self.button_logout.grid(row=2, column=1)
        self.user_list.insert(END, "在线用户", 'orange')
        self.user_list.configure(state='disabled')
        self.text_msglist.configure(state='disabled')


    def send_message(self):

        content = self.text_msg.get("1.0", END)
        self.app.logger.info("content: %s" % content)
        if not isinstance(self.app, Application):
            exit(-1)

        def send_callback(message):
            if message[0].type() != 'SUCCESS':
                self.app.logger.error("Send Failed")

        self.app.send(content, send_callback)
        self.text_msg.delete('0.0', END)

    def update_user_list(self, user_list):
        self.user_list.configure(state='normal')
        self.user_list.delete(0.0, END)
        self.user_list.insert(END, "在线用户\n\n", 'green')
        for user in user_list:
            self.user_list.insert(END, '%s\n' % user, 'black')
        self.user_list.configure(state='disabled')

    def update_messages(self, messages):

        self.text_msglist.configure(state='normal')
        for msg in messages:
            content = '%s:' % msg.get_username() + time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()) + '\n'
            self.text_msglist.insert(END, content, 'green')
            self.text_msglist.insert(END, msg.get_content(), 'black')
        self.app.logger.info('Update messages')
        self.text_msglist.configure(state='disabled')

    def logout(self):

        if tkMessageBox.askyesno('提示信息', '确认要退出吗？'):
            print("logout")
            self.app.logout()

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

    def update_info(self):

        def update_message(messages):
            if isinstance(self.current_view, ChatView):
                self.current_view.update_messages(messages)

        def update_users(msg):
            msg = msg[0].get_content()
            users = msg.split(',')
            if isinstance(self.current_view, ChatView):
                self.current_view.update_user_list(users)

        def request_update():

            global timer

            self.client.update_message(update_message)
            self.client.update_users(update_users)

            timer = threading.Timer(2, request_update)
            timer.start()

        timer = threading.Timer(1, request_update)
        timer.start()

    def start(self):

        self.current_view = LoginView(self, self.root)
        self._resize_window(300, 200)
        self.root.mainloop()

    def send(self, msg_str, callback):
        self.client.send_message(msg_str, callback)

    def open_chat(self):
        self.current_view.destroy()
        self._resize_window(420, 425)
        self.current_view = ChatView(self)
        p = thread.start_new_thread(self.update_info, ())

    def logout(self):
        self.current_view.destroy()

        def callback(message):
            if message[0].type() == 'SUCCESS':
                self.logger.info('[Client] Quit')
                self.root.destroy()
                thread.exit_thread()
        self.client.logout(callback)


def test():

    app = Application()
    app.start()


test()
