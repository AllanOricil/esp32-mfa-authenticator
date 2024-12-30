# YML Parser

- 18203 characters
- max number of services: 113
- secret length: 64
- service name length: 60
- group length: 1
- sample:

```yml
services:
  - name: TM64FT9HA7FMB3YMZRJYE7YKGZJSRIPHNB4OZWT2QDCABQ7S45M4P3ZC7PJA
    secret: GM64FNHBO2DOB3YMZRJYE7YKGZ6VZX3P6H4OZWT2QDCABQ79K2HI1GUM8PJAOZVW
    group: 0
```

After parsing the file containing the services, there was still 44740 bytes available, according to `xPortGetFreeHeapSize()` output.
This issue could be bypassed with the following solutions:

- divide services.yml into multiple smaller services.yml, and parse each in sequence.
- replace the YAML parser and use a simple .txt to store services, where each line represents a single service with properties separated by commas. Then use string splitting to extract each property, and process the file line by line while streaming its content.
