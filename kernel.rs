#![no_std]
#![no_main]

use core::panic::PanicInfo;

#[no_mangle]
pub extern "C" fn kmain() -> ! {
    let vidmem = 0xb8000 as *mut u8;

    unsafe {
        *vidmem = b'a';
        *vidmem.offset(1) = 0x0F;
    }
    loop {}
}

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}
