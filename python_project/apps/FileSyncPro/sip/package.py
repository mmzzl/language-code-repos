# -*- coding: utf-8 -*-
run_args = {
    "config": {
        "ini": {
            "package.conf": [
                {
                    "extra": {
                        "multiplate": "true"
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
                        "pkg_id": "20240827"
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
                "CUSTOMER_NAME": "XFF",
                "CUSTOM_NUMBER": "SIP-20240827",
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
        "target_path": "source\\SIP\\SIP3.0.90"
    },
    "branch":  "dev-SIP-2024060301-liantong-v90",
    "cmd": [],
    "filename": "liantong",
    "user": "中国联通",
    "first": True,
    "template_commit_message": "",
    "base_commit_message": "",
    "commit_message": ""
}
