Name
====

ngx_sysinfo - get information about nginx internals.

Build status
============
[![Build Status](https://travis-ci.org/ZigzagAK/ngx_sysinfo.svg)](https://travis-ci.org/ZigzagAK/ngx_sysinfo)

Table of Contents
=================

* [Name](#name)
* [Status](#status)
* [Synopsis](#synopsis)
* [Description](#description)
* [Configuration directives](#configuration-directives)

Status
======

This library is still under early development.

Description
===========

Various information about nginx internals.

Currently contains information about shared memory usage (pages and slab).

[Back to TOC](#table-of-contents)

Synopsis
========

```nginx
http {
  server {
    listen 5555;

    location = /sysinfo {
      sysinfo;
    }
  }

  upstream test1 {
    zone shm_test1 128k;
    server 0.0.0.0;
  }

  upstream test2 {
    zone shm_test2 128k;
    server 0.0.0.0;
  }
}
```

Output
------
```
{
    "shared_memory": [
        {
            "free_pages": 28,
            "name": "shm_test1",
            "pages": 31,
            "slab": [
                {
                    "fails": 0,
                    "size": 8,
                    "total": 0,
                    "used": 0
                },
                {
                    "fails": 0,
                    "size": 16,
                    "total": 0,
                    "used": 0
                },
                {
                    "fails": 0,
                    "size": 32,
                    "total": 4064,
                    "used": 32
                },
                {
                    "fails": 0,
                    "size": 64,
                    "total": 0,
                    "used": 0
                },
                {
                    "fails": 0,
                    "size": 128,
                    "total": 4096,
                    "used": 128
                },
                {
                    "fails": 0,
                    "size": 256,
                    "total": 4096,
                    "used": 256
                },
                {
                    "fails": 0,
                    "size": 512,
                    "total": 0,
                    "used": 0
                },
                {
                    "fails": 0,
                    "size": 1024,
                    "total": 0,
                    "used": 0
                },
                {
                    "fails": 0,
                    "size": 2048,
                    "total": 0,
                    "used": 0
                }
            ],
            "summary": {
                "total": 126976,
                "used": 416
            },
            "used": "9%"
        },
        {
            "free_pages": 28,
            "name": "shm_test2",
            "pages": 31,
            "slab": [
                {
                    "fails": 0,
                    "size": 8,
                    "total": 0,
                    "used": 0
                },
                {
                    "fails": 0,
                    "size": 16,
                    "total": 0,
                    "used": 0
                },
                {
                    "fails": 0,
                    "size": 32,
                    "total": 4064,
                    "used": 32
                },
                {
                    "fails": 0,
                    "size": 64,
                    "total": 0,
                    "used": 0
                },
                {
                    "fails": 0,
                    "size": 128,
                    "total": 4096,
                    "used": 128
                },
                {
                    "fails": 0,
                    "size": 256,
                    "total": 4096,
                    "used": 256
                },
                {
                    "fails": 0,
                    "size": 512,
                    "total": 0,
                    "used": 0
                },
                {
                    "fails": 0,
                    "size": 1024,
                    "total": 0,
                    "used": 0
                },
                {
                    "fails": 0,
                    "size": 2048,
                    "total": 0,
                    "used": 0
                }
            ],
            "summary": {
                "total": 126976,
                "used": 416
            },
            "used": "9%"
        }
    ]
}
```

[Back to TOC](#table-of-contents)

Configuration directives
========================

sysinfo
-------
* **syntax**: `sysinfo`
* **context**: `location`

Register content handler.

[Back to TOC](#table-of-contents)
