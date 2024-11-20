# -*- coding: utf-8 -*-
from FileSyncPro.sip.app_base import AppBase
from context import Context
from package import run_args


class SipSyncFile(AppBase):
    def __init__(self, *args, **kwargs):
        super(SipSyncFile, self).__init__(*args, **kwargs)


if __name__ == '__main__':
    syncfile = SipSyncFile(run_args)
    syncfile.run_action()
