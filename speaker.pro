TEMPLATE = subdirs

SUBDIRS += \
    Interface \
    Control \
    Server \
    Client \
    creator_ebook


Control.depends = Interface
Server.depends = Interface Control
Client.depends = Interface Control
creator_ebook.depends = Interface Control
