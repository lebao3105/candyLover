# candyLover

> Currently most texts in this project are Vietnamese.
> Sorry for the inconvenience - this will be fixed when the bot gets feature-complete.

Một bot Discord với đầy đủ những thứ từ nhiều bot khác:
* Trapboy (bẫy bot)
* Đếm số
* Nối từ, nối cả câu
* Blacklist từ
* Vân vân và mây mây?

## Cài đặt

Hiện tại candyLover không hỗ trợ cài đặt.
Nhưng miễn là các thư viện candyLover cần vẫn tồn tại (khớp cả phiên bản và đường dẫn) thì có thể copy `build/soc` vào 1 thư mục bất kì có trong biến môi trường PATH.

## Biên dịch

Cần:
1. Git (KHÔNG TẢI SOURCE CODE DƯỚI DẠNG ARCHIVE)
2. Những yêu cầu của [concord](https://github.com/Cogmasters/concord/tree/dev)
3. Lua (**thư viện**, chưa chốt phiên bản tối thiểu)
4. Có bot token
5. Trình biên dịch hỗ trợ C23 (xem các trình biên dịch hỗ trợ tại [đây](https://cppreference.com/c/23))
6. Make
7. [Gấu của bạn](https://github.com/rizsotto/bear) để generate compile_commands.json, hỗ trợ autocomplete các thứ

Clone repo này:

```
git clone --recursive https://github.com/lebao3105/candyLover.git
# hoặc nếu có github CLI
gh repo clone lebao3105/candyLover -- --recursive
# xong 1 trong 2 lệnh trên...
cd candyLover
```

Build:

```
make all
# hoặc build và generate compile_commands.json (chỉ cần 1 lần trừ khi thêm source file mới)
make compile_commands
```

Chương trình được lưu ở build/soc.

## Setup

TODO:tm:

candyLover sử dụng Lua để setup các chức năng - không có auto-reload.

candyLover chỉ load một số module của Lua:
* Base
* String
* Math
* Table
* Xem các section từ https://www.lua.org/manual/5.5/manual.html#6.2 để biết thêm chi tiết.

### Hàm global

`include(path)`: Nạp và chạy `path` ngay lập tức.
