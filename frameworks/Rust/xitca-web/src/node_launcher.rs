use std::env;
use std::ffi::CString;
use std::net::TcpListener;
use std::os::fd::AsRawFd;

fn main() {
    let listener = TcpListener::bind("0.0.0.0:8080").unwrap();
    let sd = listener.as_raw_fd();

    env::set_var("FD_COUNT", sd.to_string());

    let file = CString::new("node").unwrap();
    let param0 = CString::new("--experimental-wasi-unstable-preview1").unwrap();
    let param1 = CString::new("/opt/xitca-web-wasm/xitca-web.mjs").unwrap();
    let f = file.as_ptr();
    let arg0 = param0.as_ptr();
    let arg1 = param1.as_ptr();
    let arg2: *const libc::c_char = std::ptr::null();

    unsafe {
        let socket_flags = libc::fcntl(sd, libc::F_GETFD) & !libc::FD_CLOEXEC;

        libc::fcntl(sd, libc::F_SETFD, socket_flags);
        libc::execlp(f, arg0, arg1, arg2);
        unreachable!();
    }
}
