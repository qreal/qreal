# (c) 2017, Iakov Kirilenko

FROM ubuntu:14.04
RUN useradd -M -d /sandbox sandbox
#RUN echo "deb http://ppa.launchpad.net/ubuntu-toolchain-r/test/ubuntu precise main" > \
    #    /etc/apt/sources.list.d/toolchain-r-test.list
#RUN apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 1E9377A2BA9EF27F
        #   &&  apt-key adv --recv-keys --keyserver keyserver.ubuntu.com 10C56D0DE9977759
RUN apt-get -y update
RUN apt-get -y install libstdc++6 xterm libegl1-mesa libpulse0 libglib2.0-0
RUN apt-get -y update
RUN apt-get -y dist-upgrade

#HOTFIX: alsa device 0 was incorrect
RUN echo "pcm.!default hw:PCH" > /root/.asoundrc
ADD trik-studio-offline-linux-gnu64-installer /
#RUN ldd /trik-studio-offline-linux-gnu64-installer --help
#mount /wads as external volume
#ENV DOOMWADDIR=/wads
#CMD
