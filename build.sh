mkdir -p ./Build && rm -rf ./Build/*

if [ -z "$(docker images -q sdl-grabber 2> /dev/null)" ]; then
    docker build -t sdl-grabber ./
else
    echo 'Skipping build'
fi

docker run \
	-v $(pwd)/Build:/project/Build \
	-v $(pwd)/Source:/project/Source \
	sdl-grabber
