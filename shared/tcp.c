
//
//
// /* write a string to the tcp connection  */
// void tcp_write(char *message) {
//
//     // initialize write buffer
//     uv_buf_t buf;
//
//     // set length of buffer to message length
//     buf.len = strlen(message);
//
//     // assign base to message
//     buf.base = message;
//
//     // allocate write request
//     uv_write_t* write_req = (uv_write_t*)malloc(sizeof(uv_write_t));
//
//     // perform write 
//     uv_write(write_req, req->handle, &buf, 1, on_write_end);
// }
//
