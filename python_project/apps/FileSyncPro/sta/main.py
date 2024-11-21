# -*- coding: utf-8 -*-
import sys
import os
base_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.append(base_dir)
from app import AppBase
from package import run_args


class StaSyncFile(AppBase):
    def __init__(self, *args, **kwargs):
        super(StaSyncFile, self).__init__(*args, **kwargs)


if __name__ == '__main__':
    syncfile = StaSyncFile(run_args)
    syncfile.run_action()
