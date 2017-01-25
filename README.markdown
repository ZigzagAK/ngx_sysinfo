Name
====

ngx_sysinfo - get information about nginx internals.

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

Currently contains information about shared memory usage (only free pages).

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
    "zone1": "3%",
    "zone2": "3%",
    "zone3": "1%",
    "zone4": "1%"
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
