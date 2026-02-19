#include "../client.h"

fr_handler create_frame_handler(init_handler init, input_handler input, render_handler render) {
	return (fr_handler) {
		.init = init,
		.input = input,
		.render = render,
	};
}

