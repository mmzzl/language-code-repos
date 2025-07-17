# -*- coding: utf-8 -*-

class Instance(object):
    def __init__(self, data):
        self._data = data

    def __getattr__(self, key):
        value = self._data.get(key)
        if isinstance(value, dict):
            return Instance(value)
        elif isinstance(value, list):
            return [Instance(item) if isinstance(item, dict) else item for
                    item in value]
        else:
            return value

    def to_dict(self):
        return self._data

    # 可选：让 json.dumps 能识别此对象
    def __json__(self):
        return self.to_dict()
