# hang-stdin

`hang-stdin` makes command reading stdin would be block forever. 
This behavior will be equivalent to read the empty pipe.

## Build and Install

Here is a way to build and install it.

1. generate build folder

```sh
cmake -S . -B ./build
```
2. start build

```sh
cmake --build ./build
```

3. install

```sh
cmake --install ./build --prefix /usr/local/ # root permission required
```

## Usage

`hang-stdin` will requires the target command path and its arguments.

### Behave as `sleep infinity`

```sh
hang-stdin /usr/bin/cat
```

It will block current shell forever due to blocking read.

### Keep WSL distro alive

If WSL has `systemd` enabled, the distro will be terminated when 
no such child process run under WSL init process. This will make some gui applications terminated
after last WSL terminal closed. It requires some process running with
WSL init process to prevent WSL distro down. WSLInterop features will allow you to run Windows
binaries in WSL which will also run WSL init process. 
And [choice](https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/choice) 
is builtin command in Windows which can wait stdin. Hence, we can write such `wsl-keepalive.service` file to 
keep the distro running. 

```service
[Unit]
Description=WSL Keep Distro Alive

[Service]
ExecStart=/usr/local/bin/hang-stdin /mnt/c/Windows/System32/choice.exe /n

[Install]
WantedBy=multi-user.target
```

Save it under the path `/etc/systemd/system/` and then run systemd command
with `root` permission to control liveness.

* `start/stop` service will make current distro whether keep alive.
```sh
systemctl [start|stop] wsl-keepalive.service
```

* If you want the service autostart in next distro boot, you can `enable` the service.
```sh
systemctl enable wsl-keepalive.service
```
