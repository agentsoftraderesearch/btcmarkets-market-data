FROM debian:bookworm

RUN apt-get update && apt-get install -y \
    build-essential \
    libcurl4-openssl-dev \
    libcjson-dev \
    libcriterion-dev \
    git \
    && rm -rf /var/lib/apt/lists/*


RUN useradd -ms /bin/bash devuser
WORKDIR /home/devuser/project
USER devuser

CMD ["/bin/bash"]