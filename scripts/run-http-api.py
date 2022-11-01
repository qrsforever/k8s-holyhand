#!/usr/bin/python3
# -*- coding: utf-8 -*-

# @file run-http-api.py
# @brief
# @author QRS
# @version 1.0
# @date 2022-11-01 19:23


import json

from flask import Flask, request
from flask_cors import CORS

app = Flask(__name__)
CORS(app, supports_credentials=True)


@app.route('/cosftpd/on_upload', methods=['POST'], endpoint='on_upload')
def _cosftpd_upload():
    reqjson = json.loads(request.get_data().decode())
    print(reqjson)
    return '', 200


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=9876)
