# candyLover

English version [here](README.en.md).

Một bot Discord với đầy đủ những thứ từ nhiều bot khác:
* Trapboy (bẫy bot)
* Đếm số
* Nối từ
* Blacklist từ
* Vân vân và mây mây?

Đa số sẽ có lệnh reset.

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

candyLover sử dụng Lua để setup các chức năng - không có auto-reload.

Chỉ có một số module của Lua là được load (có thể thay đổi tùy theo nhu cầu của bot sau này):
* Base
* String
* Math
* Table

Xem https://www.lua.org/manual/5.5/manual.html#6.2 để biết thêm chi tiết.

Mặc định bot sẽ load ./env.lua để setup chức năng, sử dụng cờ --env-path / -e để sử dụng file khác.

### Hàm global

* `include(path)`: Nạp và chạy `path` ngay lập tức;
* Vì candyLover có load 1 số module Lua, nên các hàm từ module đó cũng có thể sử dụng;
* Các hàm bắt đầu section bên dưới;
* `end_section()`: kết thúc 1 section. Không bắt buộc vì:
    - candyLover khi bắt đầu 1 section mới không quan tâm đến section trước đó
    - Hàm của section nào sẽ chỉ chạy trong section đó, và sẽ báo lỗi khi cần

### Section

Có *[REDACTED]* section:
* environment: Setup môi trường trước khi bot thực hiện công việc. Thường là đặt ID kênh, log level v.v
* count: Tính năng đếm số, bắt đầu từ 1 số bất kì (mặc định là 1). Không ai được phép đếm 2 lần liên tiếp. Một số phép toán (cộng trừ nhân chia mũ) có thể được sử dụng (vì lý do bảo mật, không được phép chạy hàm cho đến khi có ai đủ thời gian để kiểm tra đầu vào). Mọi sai lầm đều phải trả giá:penguin:
* trapboy: Bẫy spam. Hiện chưa có kế hoạch nào mới...
* wordchain: Nối từ.

Mọi section đều có thể được kết thúc bằng hàm *end_section()*