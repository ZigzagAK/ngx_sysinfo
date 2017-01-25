Name
====

ngx_sysinfo - Get informations about nginx internals.

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
Currently contains information about shared memory usage.

[Back to TOC](#table-of-contents)

Synopsis
========

```nginx
http {
  listen 5555;

  location = /sysinfo {
    sysinfo;
  }
}
```

Output
------
```
{
  "used": "46Mb",
  "total": "1054Mb",
  "usage": {
    "quota_remains": "3%",
    "quota_addinfo": "3%",
    "quota_hits": "1%",
    "quota_locks": "1%",
    "quota_ids_lim": "3%",
    "resources_cache": "13%",
    "limit_req_limit": "3%",
    "resources_locks": "1%",
    "resources_queue": "1%",
    "limit_req_queue": "9%"
  }
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
