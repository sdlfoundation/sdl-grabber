FROM devkitpro/devkitppc

ENV DKP /opt/devkitpro
ENV PACMAN /opt/devkitpro/pacman/bin/pacman
ENV CMAKE /opt/devkitpro/portlibs/wiiu/bin/powerpc-eabi-cmake

WORKDIR /project

COPY ./Dependencies Dependencies

RUN apt-get update
RUN apt-get install -y python3-pip libquadmath0 libquadmath0-ppc64el-cross gcc libpthread-stubs0-dev libboost-thread-dev
RUN pip install cmake --upgrade
RUN ${PACMAN} --noconfirm -Syu
RUN ${PACMAN} --noconfirm -S wiiu-dev wut
RUN rm -rf /var/lib/apt/lists/* /var/cache/apt/archives /tmp/* /var/tmp/*

# Dependencies
RUN cd /project/Dependencies && mkdir wups/Build && mkdir sdl-utils/Build

RUN cd /project/Dependencies/macchiato/Dependencies/libkernel && make install
#RUN cd /project/Dependencies/macchiato/Dependencies/curl/Build && $CMAKE .. && cd .. && cmake --build Build

#RUN cd /project/Dependencies/macchiato/Build && $CMAKE .. && cd .. && cmake --build Build
#RUN cd /project/Dependencies/macchiato/Build && ctest

RUN cd /project/Dependencies/wups/Build && $CMAKE .. && cd .. && cmake --build Build
RUN cd /project/Dependencies/sdl-utils/Build && $CMAKE .. && cd .. && cmake --build Build
RUN cd /project/Dependencies/libnotifications && make install

RUN cd /project/Dependencies && \
    cp -r wups $DKP && \
    cp -r sdl-utils $DKP && \
    cp -r macchiato/Dependencies/libkernel $DKP && \
    cp -r libnotifications $DKP

COPY ./Source Source
COPY ./Build Build
COPY ./Makefile Makefile

CMD export PATH=/opt/devkitpro/portlibs/wiiu/bin:$PATH && \
	cd /project && \
	make NDEBUG=1 && \
    cp sdl-grabber.wps Build/
