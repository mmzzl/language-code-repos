# -*- coding: utf-8 -*-
run_args = {
    "config": {
        "ini": {
            "package.conf": [
                {
                    "extra": {
                        "multiplat": "true"
                    }
                },
                {
                    "meta": {
                        "from_version": "3.0.92",
                        "to_version": "3.0.92"
                    }
                },
                {
                    "package_info": {
                        "pkg_id": "20241119"
                    }
                }
            ]
        },
        "txt": {
            "pkgrc": {
                "LEVEL": "3",
                "MAJORVER": "3",
                "MINORVER": "0.92",
                "RVERSION": "",
                "CUSTOMER_NAME": "haiguangxingxi",
                "CUSTOM_NUMBER": "SIP-2024111501",
                "RESTRICT_APPVERSION": "SIS3.0.92.0  Build20240726",
                "PACKAGE_AUTHOR": "gxs",
                "EFFECT_APP": "0",
                "RESTART_FANTOM": "1"
            }
        },
        "sh": {
            "appsh3": {
                "start_write_line": "bash /home/fantom/apps/secvisual/bin/recover_icon/chown_copyright_logo.sh &>/dev/null",
                "cmd": []
            }
        },
    },
    "path": {
        "sip_master_path": "D:\\sangfor\\new\\sip-master-new",
        "sip_customize_path": "D:\\sangfor\\sip-customize",
        "temp_path": "template\\custom_template\\SIP",
        "target_path": "source\\SIP\\SIP3.0.92"
    },
    "branch":  "dev-SIP-2024111501-haiguangxingxi-v92",
    "cmd": [],
    "filename": "haiguangxingxi",
    "user": "海光信息技术股份有限公司",
    "first": True,
    "version": "sip-master-new",
    "template_commit_message": "[ADD]定制模板",
    "base_commit_message": "ADD]基线代码提交",
    "commit_message": "[ADD]海光信息技术股份有限公司atrust日志解析"
}
