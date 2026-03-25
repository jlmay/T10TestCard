// -*- mode:c++ -*-

/**
 * @file   dcrf32.h
 * @author WeiYang <604710872@qq.com>
 * @date   Wed Jan 20 10:00:00 2019
 * @brief  读写器功能接口声明文件。
 */

#ifndef DCRF32_H_
#define DCRF32_H_

#ifdef WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#define API_ATTRIBUTE
#define USER_API __stdcall
#pragma pack(push)
#pragma pack(1)
#else
typedef int HANDLE;
#define API_ATTRIBUTE __attribute__((visibility("default")))
#define USER_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * @brief  库入口。
   * @par    说明：
   * 可以获取或设置一些库相关参数，此接口可不掉用，如需调用必须放在所有其它接口之前调用。
   * @param[in] flag 标志，用于决定 @a context 的类型和含义。
   * @n 0 - 表示获取库版本， @a context 类型为char *，请至少分配64个字节。
   * @n 1 - 表示设置库的工作目录， @a context 类型为const char *。
   * @n 2 - 表示设置库调用者的工作目录， @a context 类型为const char *。
   * @n 3 - 保留内部使用，表示注入端口辅助接口上下文， @a context 类型为自定义结构指针。
   * @n 4 - 保留内部使用，表示获取库的外部认证接口上下文， @a context 类型为自定义结构指针。
   * @n 5 - 保留内部使用，设置库的版本， @a context 类型为const char *。
   * @n 6 - 保留内部使用，表示注入日志辅助接口上下文， @a context 类型为自定义结构指针。
   * @n 7 - 保留内部使用，使得注入接口库可以获取控制上下文， @a context 类型为自定义结构指针。
   * @n 8 - 保留内部使用，进行库的内部认证。
   * @param[in,out] context 参数实际类型和含义由 @a flag 的值来决定。
   */
  API_ATTRIBUTE void USER_API LibMain(int flag, void *context);

  /**
   * @brief  释放资源。
   * @par    说明：
   * 用于释放之前分配或占用的资源。
   */
  API_ATTRIBUTE void USER_API dc_release();

  /**
   * @brief  配置端口名称。
   * @par    说明：
   * 用于配置端口对应的物理名称，在调用 ::dc_init 之前可以使用此接口来改变端口号内部对应的默认物理名称，如Windows平台下0端口号对应的默认名称为"COM1"，Linux平台下0端口号对应的默认名称为"/dev/ttyS0"。
   * @param[in] port 端口号，同 ::dc_init 的 @a port 。
   * @param[in] name 物理名称。
   */
  API_ATTRIBUTE void USER_API dc_config_port_name(short port, const char *name);

  /**
   * @brief  配置端口共享模式。
   * @par    说明：
   * 用于配置端口的共享模式，在调用 ::dc_init 之前可以使用此接口来改变端口打开所使用的共享模式，注意：共享模式的使用和操作系统平台支持的端口操作模式依赖相关，通常情况不建议使用。
   * @param[in] port 端口号，同 ::dc_init 的 @a port 。
   * @param[in] mode 模式，0表示不共享（默认行为），1表示共享。
   */
  API_ATTRIBUTE void USER_API dc_config_port_share_mode(short port, int mode);

  /**
   * @brief  配置端口初始化超时。
   * @par    说明：
   * 用于配置端口的初始化超时时间，在调用 ::dc_init 之前可以使用此接口来改变端口打开所使用的超时时间，注意：超时时间的使用和操作系统平台支持的端口操作模式依赖相关，通常情况不建议使用。
   * @param[in] port 端口号，同 ::dc_init 的 @a port 。
   * @param[in] timeout 超时时间，单位为毫秒。
   */
  API_ATTRIBUTE void USER_API dc_config_port_init_timeout(short port, int timeout);

  /**
   * @brief  打开设备。
   * @par    说明：
   * 建立设备的通讯并且分配相应的资源，大部分功能接口都需要在此过程后才能进行，在不需要使用设备后，必须使用 ::dc_exit 去关闭设备的通讯和释放资源。
   * @param[in] port 端口号。
   * @n 0~99 - 表示串口模式（编号物理对应），编号0表示第一个串口合法设备，编号1表示第二个串口合法设备，以此类推。
   * @n 100~199 - 表示USB模式（编号逻辑对应），编号100表示第一个USB合法设备，编号101表示第二个USB合法设备，以此类推。
   * @n 200~299 - 表示PCSC模式（编号逻辑对应），编号200表示第一个PCSC合法设备，编号201表示第二个PCSC合法设备，以此类推。
   * @n 300~399 - 表示蓝牙模式（编号逻辑对应），编号300表示第一个蓝牙合法设备，编号301表示第二个蓝牙合法设备，以此类推。
   * @n 400~499 - 表示虚拟网络模式（编号逻辑对应）。
   * @n 500~599 - 表示内置SPI模式（编号逻辑对应）。
   * @n 600~699 - 表示网络模式（编号逻辑对应）。
   * @n 700~799 - 表示FTDI模式（编号逻辑对应）。
   * @param[in] baud 波特率，只针对串口模式有效。
   * @return <0表示失败，否则为设备标识符。
   */
  API_ATTRIBUTE HANDLE USER_API dc_init(short port, int baud);

  /**
   * @brief  打开设备。
   * @par    说明：
   * 建立设备的通讯并且分配相应的资源，大部分功能接口都需要在此过程后才能进行，在不需要使用设备后，必须使用 ::dc_exit 去关闭设备的通讯和释放资源。
   * @param[in] port 端口号。
   * @n 0~99 - 表示串口模式（编号物理对应），编号0表示第一个串口合法设备，编号1表示第二个串口合法设备，以此类推。
   * @n 100~199 - 表示USB模式（编号逻辑对应），编号100表示第一个USB合法设备，编号101表示第二个USB合法设备，以此类推。
   * @n 200~299 - 表示PCSC模式（编号逻辑对应），编号200表示第一个PCSC合法设备，编号201表示第二个PCSC合法设备，以此类推。
   * @n 300~399 - 表示蓝牙模式（编号逻辑对应），编号300表示第一个蓝牙合法设备，编号301表示第二个蓝牙合法设备，以此类推。
   * @n 400~499 - 表示虚拟网络模式（编号逻辑对应）。
   * @n 500~599 - 表示内置SPI模式（编号逻辑对应）。
   * @n 600~699 - 表示网络模式（编号逻辑对应）。
   * @n 700~799 - 表示FTDI模式（编号逻辑对应）。
   * @param[in] baud 波特率，只针对串口模式有效。
   * @param[in] name 设备逻辑名称。
   * @return <0表示失败，否则为设备标识符。
   */
  API_ATTRIBUTE HANDLE USER_API dc_init_name(short port, int baud, const char *name);

  /**
   * @brief  关闭设备。
   * @par    说明：
   * 关闭设备的通讯和释放资源。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_exit(HANDLE icdev);

  /**
   * @brief  获取名称。
   * @par    说明：
   * 此接口将获取一个用于识别当前打开设备与软件层对应的逻辑名称，获取的逻辑名称仅供内部使用。
   * @param[in] icdev 设备标识符。
   * @param[out] name 返回的名称字符串，请至少分配64个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_getname(HANDLE icdev, char *name);

  /**
   * @brief  获取USB ID信息。
   * @par    说明：
   * 此接口将获取当前打开设备所使用USB端口的VID和PID，如果打开的设备不是USB设备，将返回失败。
   * @param[in] icdev 设备标识符。
   * @param[out] vid USB端口的VID。
   * @param[out] pid USB端口的PID。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_getusbid(HANDLE icdev, unsigned short *vid, unsigned short *pid);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_config(HANDLE icdev, unsigned char _Mode, unsigned char _Baud);

  /**
   * @brief  寻卡请求。
   * @par    说明：
   * 支持ISO 14443 Type A类型卡片的寻卡请求。
   * @param[in] icdev 设备标识符。
   * @param[in] _Mode 模式，0x00表示对空闲卡进行操作，0x01表示对所有卡操作。
   * @param[out] TagType 返回的ATQA值。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  API_ATTRIBUTE short USER_API dc_request(HANDLE icdev, unsigned char _Mode, unsigned short *TagType);

  /**
   * @brief  防卡冲突。
   * @par    说明：
   * 支持ISO 14443 Type A类型卡片的防卡冲突。
   * @param[in] icdev 设备标识符。
   * @param[in] _Bcnt 保留，固定为0x00。
   * @param[out] _Snr 返回的卡序列号。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_anticoll(HANDLE icdev, unsigned char _Bcnt, unsigned int *_Snr);

  /**
   * @brief  选卡操作。
   * @par    说明：
   * 通过指定序列号，选取相应的卡片。
   * @param[in] icdev 设备标识符。
   * @param[in] _Snr 卡片序列号。
   * @param[out] _Size 返回的SAK值。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_select(HANDLE icdev, unsigned int _Snr, unsigned char *_Size);

  /**
   * @brief  验证M1卡密码。
   * @par    说明：
   * 使用设备内部装载的密码来验证M1卡密码。
   * @param[in] icdev 设备标识符。
   * @param[in] _Mode 模式。
   * @n 0x00 - 表示用设备内部装载的第0套A密码来验证当前选取卡片的A密码。
   * @n 0x01 - 表示用设备内部装载的第1套A密码来验证当前选取卡片的A密码。
   * @n 0x02 - 表示用设备内部装载的第2套A密码来验证当前选取卡片的A密码。
   * @n 0x04 - 表示用设备内部装载的第0套B密码来验证当前选取卡片的B密码。
   * @n 0x05 - 表示用设备内部装载的第1套B密码来验证当前选取卡片的B密码。
   * @n 0x06 - 表示用设备内部装载的第2套B密码来验证当前选取卡片的B密码。
   * @param[in] _SecNr 要验证密码的扇区号。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_authentication(HANDLE icdev, unsigned char _Mode, unsigned char _SecNr);

  /**
   * @brief  终止卡操作。
   * @par    说明：
   * 使卡片进入终止状态，此时必须把卡移出感应区后再次放入感应区才能寻到这张卡。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_halt(HANDLE icdev);

  /**
   * @brief  读卡数据。
   * @par    说明：
   * 读取卡内数据，对于M1卡，一次读取一个块的数据，为16个字节；对于ML卡，一次读取相同属性的两页，为8个字节。
   * @param[in] icdev 设备标识符。
   * @param[in] _Adr 地址。
   * @n M1卡 - S50块地址（0~63），S70块地址（0~255）。
   * @n ML卡 - 页地址（0~11）。
   * @param[out] _Data 固定返回16个字节数据，真实数据可能小于16个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_read(HANDLE icdev, unsigned char _Adr, unsigned char *_Data);

  /**
   * @brief  读卡数据。
   * @par    说明：
   * ::dc_read 的HEX形式接口，参数 @a _Data 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_read_hex(HANDLE icdev, unsigned char _Adr, char *_Data);

  /**
   * @brief  写卡数据。
   * @par    说明：
   * 写入数据到卡片内，对于M1卡，一次必须写入一个块的数据，为16个字节；对于ML卡，一次必须写入一个页的数据，为4个字节。
   * @param[in] icdev 设备标识符。
   * @param[in] _Adr 地址。
   * @n M1卡 - S50块地址（1~63），S70块地址（1~255）。
   * @n ML卡 - 页地址（2~11）。
   * @param[in] _Data 固定传入16个字节数据，真实数据可能小于16个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_write(HANDLE icdev, unsigned char _Adr, unsigned char *_Data);

  /**
   * @brief  写卡数据。
   * @par    说明：
   * ::dc_write 的HEX形式接口，参数 @a _Data 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_write_hex(HANDLE icdev, unsigned char _Adr, char *_Data);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_write_TS(HANDLE icdev);

  /**
   * @brief  装载设备密码。
   * @par    说明：
   * 装载M1卡密码到设备内部，装载后可以在特定时候用 ::dc_authentication 来验证M1卡密码。注意：由于多次装载设备密码可能会受硬件存储寿命限制，此接口只用于密码相对固定，装载次数少的场合。
   * @param[in] icdev 设备标识符。
   * @param[in] _Mode 模式，同 ::dc_authentication 的 @a _Mode 。
   * @param[in] _SecNr 对应装载的扇区号。
   * @param[in] _NKey 写入设备中的卡密码，固定为6个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_load_key(HANDLE icdev, unsigned char _Mode, unsigned char _SecNr, unsigned char *_NKey);

  /**
   * @brief  装载设备密码。
   * @par    说明：
   * ::dc_load_key 的HEX形式接口，参数 @a _NKey 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_load_key_hex(HANDLE icdev, unsigned char _Mode, unsigned char _SecNr, char *_NKey);

  /**
   * @brief  寻卡请求、防卡冲突、选卡操作。
   * @par    说明：
   * 内部包含了 ::dc_request ::dc_anticoll ::dc_select 的功能。
   * @param[in] icdev 设备标识符。
   * @param[in] _Mode 模式，同 ::dc_request 的 @a _Mode 。
   * @param[out] _Snr 返回的卡序列号。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  API_ATTRIBUTE short USER_API dc_card(HANDLE icdev, unsigned char _Mode, unsigned int *_Snr);

  /**
   * @brief  寻卡请求、防卡冲突、选卡操作。
   * @par    说明：
   * ::dc_card 的HEX形式接口，参数 @a snrstr 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_card_hex(HANDLE icdev, unsigned char _Mode, unsigned char *snrstr);

  /**
   * @brief  M1卡密码配置块操作。
   * @par    说明：
   * 修改M1卡密码配置块数据，M1卡密码配置块也就是每个扇区的最后一块，包含密码A、控制字节、密码B数据。
   * @param[in] icdev 设备标识符。
   * @param[in] _SecNr 扇区号。
   * @param[in] _KeyA 密码A，固定为6个字节。
   * @param[in] _B0 块0控制字（当一扇区有16块时，对应为块0~4的控制字），低3位（D2D1D0）对应C10、C20、C30。
   * @param[in] _B1 块1控制字（当一扇区有16块时，对应为块5~9的控制字），低3位（D2D1D0）对应C11、C21、C31。
   * @param[in] _B2 块2控制字（当一扇区有16块时，对应为块10~14的控制字），低3位（D2D1D0）对应C12、C22、C32。
   * @param[in] _B3 块3控制字（当一扇区有16块时，对应为块15的控制字），低3位（D2D1D0）对应C13、C23、C33。
   * @param[in] _Bk 保留，固定为0x00。
   * @param[in] _KeyB 密码B，固定为6个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_changeb3(HANDLE icdev, unsigned char _SecNr, unsigned char *_KeyA, unsigned char _B0, unsigned char _B1, unsigned char _B2, unsigned char _B3, unsigned char _Bk, unsigned char *_KeyB);

  /**
   * @brief  M1卡密码配置块操作。
   * @par    说明：
   * ::dc_changeb3 的HEX形式接口，参数 @a _KeyA @a _KeyB 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_changeb3_hex(HANDLE icdev, unsigned char _SecNr, const char *_KeyA, unsigned char _B0, unsigned char _B1, unsigned char _B2, unsigned char _B3, unsigned char _Bk, const char *_KeyB);

  /**
   * @brief  M1卡值块回传。
   * @par    说明：
   * 用于把指定的值块内容暂存，后续可以使用 ::dc_transfer 将暂存值块内容传递到另一块中，实现块与块之间数值传送。
   * @param[in] icdev 设备标识符。
   * @param[in] _Adr 要回传的块地址。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_restore(HANDLE icdev, unsigned char _Adr);

  /**
   * @brief  M1卡值块传递。
   * @par    说明：
   * 用于把 ::dc_restore 暂存的值块内容传递到指定块中，实现块与块之间数值传送。
   * @param[in] icdev 设备标识符。
   * @param[in] _Adr 要传递的块地址。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_transfer(HANDLE icdev, unsigned char _Adr);

  /**
   * @brief  M1卡值块加值。
   * @par    说明：
   * 用于操作M1卡值块，使块值增加指定数值。
   * @param[in] icdev 设备标识符。
   * @param[in] _Adr 块地址。
   * @param[in] _Value 要增加的数值。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_increment(HANDLE icdev, unsigned char _Adr, unsigned int _Value);

  /**
   * @brief  M1卡值块减值。
   * @par    说明：
   * 用于操作M1卡值块，使块值减少指定数值。
   * @param[in] icdev 设备标识符。
   * @param[in] _Adr 块地址。
   * @param[in] _Value 要减少的数值。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_decrement(HANDLE icdev, unsigned char _Adr, unsigned int _Value);

  /**
   * @brief  M1卡值块初始化。
   * @par    说明：
   * 用于操作M1卡，使得指定块为值块，并且初始化此值块为指定数值。
   * @param[in] icdev 设备标识符。
   * @param[in] _Adr 块地址。
   * @param[in] _Value 初始化数值。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_initval(HANDLE icdev, unsigned char _Adr, unsigned int _Value);

  /**
   * @brief  M1卡值块读值。
   * @par    说明：
   * 用于读取M1卡值块数值。
   * @param[in] icdev 设备标识符。
   * @param[in] _Adr 要读取的块地址。
   * @param[out] _Value 返回的数值。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_readval(HANDLE icdev, unsigned char _Adr, unsigned int *_Value);

  /**
   * @brief  ML卡值初始化。
   * @par    说明：
   * 用于操作ML卡，初始化卡内容为指定数值。
   * @param[in] icdev 设备标识符。
   * @param[in] _Value 初始化数值。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_initval_ml(HANDLE icdev, unsigned short _Value);

  /**
   * @brief  ML卡读值。
   * @par    说明：
   * 用于读取ML卡数值。
   * @param[in] icdev 设备标识符。
   * @param[out] _Value 返回的数值。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_readval_ml(HANDLE icdev, unsigned short *_Value);

  /**
   * @brief  ML卡减值。
   * @par    说明：
   * 用于操作ML卡值，使值减少指定数值。
   * @param[in] icdev 设备标识符。
   * @param[in] _Value 要减少的数值。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_decrement_ml(HANDLE icdev, unsigned short _Value);

  /**
   * @brief  验证M1卡密码。
   * @par    说明：
   * 使用设备内部装载的密码来验证M1卡密码。
   * @param[in] icdev 设备标识符。
   * @param[in] _Mode 模式，同 ::dc_authentication 的 @a _Mode 。
   * @param[in] KeyNr 要验证密码的扇区号。
   * @param[in] Adr 要验证密码扇区中的块号，此块号非卡片绝对块号。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_authentication_2(HANDLE icdev, unsigned char _Mode, unsigned char KeyNr, unsigned char Adr);

  /**
   * @brief  第二级防卡冲突。
   * @par    说明：
   * 支持ISO 14443 Type A类型卡片的第二级防卡冲突。
   * @param[in] icdev 设备标识符。
   * @param[in] _Bcnt 保留，固定为0x00。
   * @param[out] _Snr 返回的卡序列号。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_anticoll2(HANDLE icdev, unsigned char _Bcnt, unsigned int *_Snr);

  /**
   * @brief  第二级选卡操作。
   * @par    说明：
   * 通过指定序列号，选取相应的卡片。
   * @param[in] icdev 设备标识符。
   * @param[in] _Snr 卡片序列号。
   * @param[out] _Size 返回的SAK值。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_select2(HANDLE icdev, unsigned int _Snr, unsigned char *_Size);

  /**
   * @brief  第三级防卡冲突。
   * @par    说明：
   * 支持ISO 14443 Type A类型卡片的第三级防卡冲突。
   * @param[in] icdev 设备标识符。
   * @param[in] _Bcnt 保留，固定为0x00。
   * @param[out] _Snr 返回的卡序列号。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_anticoll3(HANDLE icdev, unsigned char _Bcnt, unsigned int *_Snr);

  /**
   * @brief  第三级选卡操作。
   * @par    说明：
   * 通过指定序列号，选取相应的卡片。
   * @param[in] icdev 设备标识符。
   * @param[in] _Snr 卡片序列号。
   * @param[out] _Size 返回的SAK值。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_select3(HANDLE icdev, unsigned int _Snr, unsigned char *_Size);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_HL_write(HANDLE icdev, unsigned char _Mode, unsigned char _Adr, unsigned int *_Snr, unsigned char *_Data);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_HL_writehex(HANDLE icdev, unsigned char _Mode, unsigned char _Adr, unsigned int *_Snr, unsigned char *_Data);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_HL_read(HANDLE icdev, unsigned char _Mode, unsigned char _Adr, unsigned int _Snr, unsigned char *_Data, unsigned int *_NSnr);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_HL_readhex(HANDLE icdev, unsigned char _Mode, unsigned char _Adr, unsigned int _Snr, unsigned char *_Data, unsigned int *_NSnr);

  /**
   * @brief  高层验证M1卡密码。
   * @par    说明：
   * 内部包含了 ::dc_card ::dc_authentication 的功能。
   * @param[in] icdev 设备标识符。
   * @param[in] reqmode 寻卡请求模式，同 ::dc_request 的 @a _Mode 。
   * @param[in] snr 保留，固定为0。
   * @param[in] authmode 验证模式，同 ::dc_authentication 的 @a _Mode 。
   * @param[in] secnr 要验证密码的扇区号。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  API_ATTRIBUTE short USER_API dc_HL_authentication(HANDLE icdev, unsigned char reqmode, unsigned int snr, unsigned char authmode, unsigned char secnr);

  /**
   * @brief  检查之前写入的数据。
   * @par    说明：
   * 内部包含了 ::dc_card ::dc_authentication ::dc_read 的功能，并且验证传入和读取的数据是否一致。
   * @param[in] icdev 设备标识符。
   * @param[in] Snr 卡序列号，用于内部核对。
   * @param[in] authmode 验证模式，同 ::dc_authentication 的 @a _Mode 。
   * @param[in] Adr 块地址。
   * @param[in] _data 块数据，固定为16个字节。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  API_ATTRIBUTE short USER_API dc_check_write(HANDLE icdev, unsigned int Snr, unsigned char authmode, unsigned char Adr, unsigned char *_data);

  /**
   * @brief  检查之前写入的数据。
   * @par    说明：
   * ::dc_check_write 的HEX形式接口，参数 @a _data 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_check_writehex(HANDLE icdev, unsigned int Snr, unsigned char authmode, unsigned char Adr, unsigned char *_data);

  /**
   * @brief  获取设备版本。
   * @par    说明：
   * 获取设备内部固件代码的版本。
   * @param[in] icdev 设备标识符。
   * @param[out] sver 返回的版本字符串，请至少分配128个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_getver(HANDLE icdev, unsigned char *sver);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_update(HANDLE icdev);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_clr_control_bit(HANDLE icdev, unsigned char _b);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_set_control_bit(HANDLE icdev, unsigned char _b);

  /**
   * @brief  复位射频。
   * @par    说明：
   * 复位设备的射频，可以关闭，关闭并且启动。
   * @param[in] icdev 设备标识符。
   * @param[in] _Msec 为0表示关闭射频，否则为复位时间，单位为10毫秒，一般调用建议值为10。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_reset(HANDLE icdev, unsigned short _Msec);

  /**
   * @brief  设备蜂鸣。
   * @par    说明：
   * 设备蜂鸣器发出指定时间的蜂鸣声。
   * @param[in] icdev 设备标识符。
   * @param[in] _Msec 蜂鸣时间，单位为10毫秒。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_beep(HANDLE icdev, unsigned short _Msec);

  /**
   * @brief  液晶显示。
   * @par    说明：
   * 显示指定字符串到液晶屏幕上。
   * @param[in] icdev 设备标识符。
   * @param[in] dispstr 要显示的字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_disp_str(HANDLE icdev, char *dispstr);

  /**
   * @brief  读EEPROM。
   * @par    说明：
   * 读取设备内部EEPROM中的数据。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移地址。
   * @param[in] length 读取长度。
   * @param[out] rec_buffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_srd_eeprom(HANDLE icdev, short offset, short length, unsigned char *rec_buffer);

  /**
   * @brief  写EEPROM。
   * @par    说明：
   * 写入数据到设备内部EEPROM中，可以用作数据保存等。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移地址。
   * @param[in] length 写入长度。
   * @param[in] send_buffer 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_swr_eeprom(HANDLE icdev, short offset, short length, unsigned char *send_buffer);

  /**
   * @brief  写EEPROM。
   * @par    说明：
   * 同 ::dc_swr_eeprom 。
   */
  API_ATTRIBUTE short USER_API swr_alleeprom(HANDLE icdev, short offset, short length, unsigned char *snd_buffer);

  /**
   * @brief  读EEPROM。
   * @par    说明：
   * 同 ::dc_srd_eeprom 。
   */
  API_ATTRIBUTE short USER_API srd_alleeprom(HANDLE icdev, short offset, short length, unsigned char *receive_buffer);

  /**
   * @brief  读EEPROM。
   * @par    说明：
   * ::dc_srd_eeprom 的HEX形式接口，参数 @a rec_buffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_srd_eepromhex(HANDLE icdev, short offset, short length, unsigned char *rec_buffer);

  /**
   * @brief  写EEPROM。
   * @par    说明：
   * ::dc_swr_eeprom 的HEX形式接口，参数 @a send_buffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_swr_eepromhex(HANDLE icdev, short offset, short length, unsigned char *send_buffer);

  /**
   * @brief  获取设备时间。
   * @par    说明：
   * 获取设备当前的时间。
   * @param[in] icdev 设备标识符。
   * @param[out] time 返回的时间，长度为7个字节，格式为'年'、'星期'、'月'、'日'、'时'、'分'、'秒'。如：0x16,0x01,0x12,0x08,0x16,0x20,0x10表示2016年星期一12月8日16时20分10秒。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_gettime(HANDLE icdev, unsigned char *time);

  /**
   * @brief  获取设备时间。
   * @par    说明：
   * ::dc_gettime 的HEX形式接口，参数 @a time 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_gettimehex(HANDLE icdev, char *time);

  /**
   * @brief  设置设备时间。
   * @par    说明：
   * 设置设备的当前时间。
   * @param[in] icdev 设备标识符。
   * @param[in] time 传入时间，长度为7个字节，格式为'年'、'星期'、'月'、'日'、'时'、'分'、'秒'。如：0x16,0x01,0x12,0x08,0x16,0x20,0x10表示2016年星期一12月8日16时20分10秒。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_settime(HANDLE icdev, unsigned char *time);

  /**
   * @brief  设置设备时间。
   * @par    说明：
   * ::dc_settime 的HEX形式接口，参数 @a time 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_settimehex(HANDLE icdev, char *time);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_setbright(HANDLE icdev, unsigned char bright);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_ctl_mode(HANDLE icdev, unsigned char mode);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_disp_mode(HANDLE icdev, unsigned char mode);

  /**
   * @brief  单DES加解密。
   * @par    说明：
   * ::dcdes 的HEX形式接口，参数 @a key @a sour @a dest 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dcdeshex(unsigned char *key, unsigned char *sour, unsigned char *dest, short m);

  /**
   * @brief  单DES加解密。
   * @par    说明：
   * 使用单DES算法对数据进行加密/解密。
   * @param[in] key 8个字节密钥。
   * @param[in] sour 8个字节要做加密/解密的数据。
   * @param[out] dest 返回8个字节运算后的数据。
   * @param[in] m 模式，0-解密，1-加密。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dcdes(unsigned char *key, unsigned char *sour, unsigned char *dest, short m);

  /**
   * @brief  指示灯点亮/熄灭。
   * @par    说明：
   * 点亮/熄灭设备的指示灯。
   * @param[in] icdev 设备标识符。
   * @param[in] _OnOff 0-点亮，1-熄灭。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_light(HANDLE icdev, unsigned short _OnOff);

  /**
   * @brief  液晶显示。
   * @par    说明：
   * 显示指定数据到液晶屏幕上。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 屏幕偏移。
   * @param[in] displen 显示数据的长度。
   * @param[in] dispstr 显示数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_high_disp(HANDLE icdev, unsigned char offset, unsigned char displen, unsigned char *dispstr);

  /**
   * @brief  设置当前接触式卡座。
   * @par    说明：
   * 设置当前接触式卡座为指定卡座，用于多卡座切换卡操作。
   * @param[in] icdev 设备标识符。
   * @param[in] _Byte 卡座编号。
   * @n 0x0C - 附卡座/接触式CPU1卡座。
   * @n 0x0B - 接触式CPU2卡座。
   * @n 0x0D - SAM1卡座。
   * @n 0x0E - SAM2卡座。
   * @n 0x0F - SAM3卡座。
   * @n 0x11 - SAM4卡座。
   * @n 0x12 - SAM5卡座。
   * @n 0x13 - SAM6卡座/ESAM芯片。
   * @n 0x14 - SAM7卡座。
   * @n 0x15 - SAM8卡座。
   * @n 0x16~0xFF - 其它卡座。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_setcpu(HANDLE icdev, unsigned char _Byte);

  /**
   * @brief  接触式CPU卡复位。
   * @par    说明：
   * 对当前卡座CPU卡进行复位操作，此复位为冷复位。
   * @param[in] icdev 设备标识符。
   * @param[out] rlen 返回复位信息的长度。
   * @param[out] databuffer 返回的复位信息，请至少分配128个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_cpureset(HANDLE icdev, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * 对当前卡座CPU卡进行指令交互操作，注意此接口不封装卡协议部分。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_cpuapdusource(HANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * 对当前卡座CPU卡进行指令交互操作，注意此接口已封装卡协议部分。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_cpuapdu(HANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  接触式CPU卡复位。
   * @par    说明：
   * ::dc_cpureset 的HEX形式接口，参数 @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_cpureset_hex(HANDLE icdev, unsigned char *rlen, char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * ::dc_cpuapdusource 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_cpuapdusource_hex(HANDLE icdev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * ::dc_cpuapdu 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_cpuapdu_hex(HANDLE icdev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * 对当前卡座CPU卡进行指令交互操作，注意此接口已封装卡协议部分，内部处理了SW1为0x61和0x6C的情况。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_cpuapdurespon(HANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * ::dc_cpuapdurespon 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_cpuapdurespon_hex(HANDLE icdev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer);

  /**
   * @brief  接触式CPU卡下电。
   * @par    说明：
   * 对当前卡座CPU卡进行下电操作。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_cpudown(HANDLE icdev);

  /**
   * @brief  设置485地址号。
   * @par    说明：
   * 设置485地址号，此设置将使得后续接口调用使用新的485地址号。
   * @param[in] saddr 地址号。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_set_addr(unsigned char saddr);

  /**
   * @brief  打开485设备。
   * @par    说明：
   * 建立设备的通讯并且分配相应的资源，大部分功能接口都需要在此过程后才能进行，在不需要使用设备后，必须使用 ::dc_exit 去关闭设备的通讯和释放资源。
   * @param[in] port 端口号。
   * @n 0~99 - 表示串口模式（编号物理对应），编号0表示第一个串口合法设备，编号1表示第二个串口合法设备，以此类推。
   * @param[in] baud 波特率。
   * @return <0表示失败，否则为设备标识符。
   */
  API_ATTRIBUTE HANDLE USER_API dc_init_485(short port, int baud);

  /**
   * @brief  设置串口485设备通讯波特率。
   * @par    说明：
   * 设置串口485设备通讯波特率。
   * @param[in] icdev 设备标识符。
   * @param[in] baud 波特率。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_changebaud_485(HANDLE icdev, int baud);

  /**
   * @brief  修改设备485地址号。
   * @par    说明：
   * 修改设备485地址号，修改后设备内部将使用新的地址号。
   * @param[in] icdev 设备标识符。
   * @param[in] saddr 地址号。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_change_addr(HANDLE icdev, unsigned char saddr);

  /**
   * @brief  非接触式CPU卡复位。
   * @par    说明：
   * 对感应区CPU卡进行复位操作。
   * @param[in] icdev 设备标识符。
   * @param[out] rlen 返回复位信息的长度。
   * @param[out] receive_data 返回的复位信息，请至少分配128个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_pro_reset(HANDLE icdev, unsigned char *rlen, unsigned char *receive_data);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * 对感应区CPU卡进行指令交互操作，注意此接口已封装卡协议部分。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @param[in] timeout 超时值，此值只在部分设备的底层使用，单位为250毫秒，一般调用建议值为7。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_pro_command(HANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  非接触式CPU卡复位。
   * @par    说明：
   * ::dc_pro_reset 的HEX形式接口，参数 @a receive_data 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_pro_resethex(HANDLE icdev, unsigned char *rlen, char *receive_data);

  /**
   * @brief  非接触式CPU卡复位。
   * @par    说明：
   * 对感应区CPU卡进行复位操作。
   * @param[in] icdev 设备标识符。
   * @param[out] rlen 返回复位信息的长度。
   * @param[out] receive_data 返回的复位信息，请至少分配128个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_pro_resetInt(HANDLE icdev, unsigned char *rlen, unsigned char *receive_data);

  /**
   * @brief  非接触式CPU卡复位。
   * @par    说明：
   * ::dc_pro_resetInt 的HEX形式接口，参数 @a receive_data 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_pro_resetInt_hex(HANDLE icdev, unsigned char *rlen, char *receive_data);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * ::dc_pro_command 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_pro_commandhex(HANDLE icdev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer, unsigned char timeout);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * 对感应区CPU卡进行指令交互操作，注意此接口不封装卡协议部分。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @param[in] timeout 超时值，此值只在部分设备的底层使用，单位为250毫秒，一般调用建议值为7。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_pro_commandsource(HANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * ::dc_pro_commandsource 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_pro_commandsourcehex(HANDLE icdev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer, unsigned char timeout);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * 对感应区CPU卡进行指令交互操作，注意此接口不封装卡协议部分。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @param[in] timeout 超时值，此值只在部分设备的底层使用，单位为250毫秒，一般调用建议值为7。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_pro_commandsource_int(HANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * ::dc_pro_commandsource_int 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_pro_commandsource_int_hex(HANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer, unsigned char timeout);

  /**
   * @brief  终止非接触式CPU卡操作。
   * @par    说明：
   * 使非接触式CPU卡进入终止状态，此时必须把卡移出感应区后再次放入感应区才能寻到这张卡。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_pro_halt(HANDLE icdev);

  /**
   * @brief  SHC1102卡寻卡请求。
   * @par    说明：
   * 支持SHC1102卡的寻卡请求。
   * @param[in] icdev 设备标识符。
   * @param[in] _Mode 模式，0x00表示对空闲卡进行操作，0x01表示对所有卡操作。
   * @param[out] TagType 返回的ATQA值。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  API_ATTRIBUTE short USER_API dc_request_shc1102(HANDLE icdev, unsigned char _Mode, unsigned short *TagType);

  /**
   * @brief  验证SHC1102卡密码。
   * @par    说明：
   * 使用传入的密码来验证SHC1102卡密码。
   * @param[in] icdev 设备标识符。
   * @param[in] _Data 密码，固定为4个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_auth_shc1102(HANDLE icdev, unsigned char *_Data);

  /**
   * @brief  读SHC1102卡。
   * @par    说明：
   * 读取SHC1102卡的数据。
   * @param[in] icdev 设备标识符。
   * @param[in] _Adr 块地址（0~15）。
   * @param[out] _Data 返回的数据，固定为4个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_read_shc1102(HANDLE icdev, unsigned char _Adr, unsigned char *_Data);

  /**
   * @brief  写SHC1102卡。
   * @par    说明：
   * 写入数据到SHC1102卡中。
   * @param[in] icdev 设备标识符。
   * @param[in] _Adr 块地址（2~15）。
   * @param[in] _Data 传入数据，固定为4个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_write_shc1102(HANDLE icdev, unsigned char _Adr, unsigned char *_Data);

  /**
   * @brief  终止SHC1102卡操作。
   * @par    说明：
   * 使SHC1102卡进入终止状态，此时必须把卡移出感应区后再次放入感应区才能寻到这张卡。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_halt_shc1102(HANDLE icdev);

  /**
   * @brief  数据转换。
   * @par    说明：
   * 普通数据换成十六进制字符串（短转长）。
   * @param[in] hex 要转换的数据。
   * @param[out] a 转换后的字符串。
   * @param[in] length 要转换数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API hex_a(unsigned char *hex, unsigned char *a, short length);

  /**
   * @brief  数据转换。
   * @par    说明：
   * 十六进制字符数据转换为普通数据（长转短）。
   * @param[in] a 要转换的数据。
   * @param[out] hex 转换后的数据。
   * @param[in] len 要转换数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API a_hex(unsigned char *a, unsigned char *hex, short len);

  /**
   * @brief  配置非接触卡类型。
   * @par    说明：
   * 配置需要操作什么类型的非接触式卡，设备上电后默认操作Type A卡，可以使用此接口来改变类型，一般在寻卡前调用此接口。
   * @param[in] icdev 设备标识符。
   * @param[in] cardtype 类型，'A'表示ISO 14443 Type A卡，'B'表示ISO 14443 Type B卡，'1'表示ISO 15693卡，'2'表示ISO 18092卡，'X'表示设备缺省行为（可能支持一个或多个类型）。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_config_card(HANDLE icdev, unsigned char cardtype);

  /**
   * @brief  寻卡请求。
   * @par    说明：
   * 支持ISO 14443 Type B类型卡片的寻卡请求。
   * @param[in] icdev 设备标识符。
   * @param[in] _Mode 保留，固定为0x00。
   * @param[in] AFI 应用族标识符。
   * @param[in] N 时间槽编号。
   * @param[out] ATQB 返回的ATQB值，请至少分配32个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_request_b(HANDLE icdev, unsigned char _Mode, unsigned char AFI, unsigned char N, unsigned char *ATQB);

  /**
   * @brief  管道标志。
   * @par    说明：
   * 支持ISO 14443 Type B类型卡片的管道标志。
   * @param[in] icdev 设备标识符。
   * @param[in] N 时间槽编号。
   * @param[out] ATQB 返回的ATQB值，请至少分配32个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_slotmarker(HANDLE icdev, unsigned char N, unsigned char *ATQB);

  /**
   * @brief  激活卡片。
   * @par    说明：
   * 支持ISO 14443 Type B类型卡片的激活。
   * @param[in] icdev 设备标识符。
   * @param[in] PUPI 伪唯一PICC标识符，固定为4个字节。
   * @param[in] CID 信道号。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_attrib(HANDLE icdev, unsigned char *PUPI, unsigned char CID);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_open_door(HANDLE icdev, unsigned char cflag);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_open_timedoor(HANDLE icdev, unsigned short utime);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_read_random(HANDLE icdev, unsigned char *data);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_write_random(HANDLE icdev, short length, unsigned char *data);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_read_random_hex(HANDLE icdev, unsigned char *data);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_write_random_hex(HANDLE icdev, short length, unsigned char *data);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_erase_random(HANDLE icdev, short length);

  /**
   * @brief  Mifare Desfire卡认证。
   * @par    说明：
   * 对Mifare Desfire卡进行认证。
   * @param[in] icdev 设备标识符。
   * @param[in] keyno 密钥号。
   * @param[in] keylen 密钥数据的长度。
   * @param[in] authkey 密钥数据。
   * @param[in] randAdata 传入随机数A，8个字节。
   * @param[out] randBdata 返回的随机数B，8个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_mfdes_auth(HANDLE icdev, unsigned char keyno, unsigned char keylen, unsigned char *authkey, unsigned char *randAdata, unsigned char *randBdata);

  /**
   * @brief  验证M1卡密码。
   * @par    说明：
   * 使用传入的密码来验证M1卡密码。
   * @param[in] icdev 设备标识符。
   * @param[in] _Mode 模式，0x00表示验证A密码，0x04表示验证B密码。
   * @param[in] _Addr 要验证密码的扇区号。
   * @param[in] passbuff 密码，固定为6个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_authentication_pass(HANDLE icdev, unsigned char _Mode, unsigned char _Addr, unsigned char *passbuff);

  /**
   * @brief  高层液晶显示。
   * @par    说明：
   * 显示指定数据到液晶屏幕上。
   * @param[in] icdev 设备标识符。
   * @param[in] dispstr 显示字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_disp_neg(HANDLE icdev, char *dispstr);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * 对感应区CPU卡进行指令交互操作，注意此接口已封装卡协议部分。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @param[in] timeout 超时值，此值只在部分设备的底层使用，单位为250毫秒，一般调用建议值为7。
   * @param[in] FG 分割值，此值只在部分设备的底层使用，单位为字节，一般调用建议值为64。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_pro_commandlink(HANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer, unsigned char timeout, unsigned char FG);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * ::dc_pro_commandlink 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_pro_commandlink_hex(HANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer, unsigned char timeout, unsigned char FG);

  /**
   * @brief  寻卡请求、防卡冲突、选卡操作。
   * @par    说明：
   * 内部包含了 ::dc_request ::dc_anticoll ::dc_select ::dc_anticoll2 ::dc_select2 的功能，此接口有设计缺陷，不能返回卡序列号实际长度，建议使用 ::dc_card_n 替代。
   * @param[in] icdev 设备标识符。
   * @param[in] _Mode 模式，同 ::dc_request 的 @a _Mode 。
   * @param[out] _Snr 返回的卡序列号，请至少分配8个字节。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  API_ATTRIBUTE short USER_API dc_card_double(HANDLE icdev, unsigned char _Mode, unsigned char *_Snr);

  /**
   * @brief  寻卡请求、防卡冲突、选卡操作。
   * @par    说明：
   * ::dc_card_double 的HEX形式接口，参数 @a _Snr 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_card_double_hex(HANDLE icdev, unsigned char _Mode, unsigned char *_Snr);

  /**
   * @brief  读ID卡。
   * @par    说明：
   * 读取ID卡数据。
   * @param[in] icdev 设备标识符。
   * @param[in] times 设备超时值，单位为秒。
   * @param[out] _Data 返回的数据，固定为5个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_read_idcard(HANDLE icdev, unsigned char times, unsigned char *_Data);

  /**
   * @brief  读取id卡。
   * @par    说明：
   * ::dc_read_idcard 的HEX形式接口，参数 @a _Data 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_read_idcard_hex(HANDLE icdev, unsigned char times, unsigned char *_Data);

  /**
   * @brief  验证M1卡密码。
   * @par    说明：
   * ::dc_authentication_pass 的HEX形式接口，参数 @a passbuff 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_authentication_pass_hex(HANDLE icdev, unsigned char _Mode, unsigned char _Addr, unsigned char *passbuff);

  /**
   * @brief  设置接触式CPU卡参数。
   * @par    说明：
   * 此接口可控制接触式CPU卡操作的行为，当不掉用时，设备将使用默认方式进行接触式CPU卡操作。
   * @param[in] icdev 设备标识符。
   * @param[in] cputype 卡座编号，同 ::dc_setcpu 的 @a _Byte 。
   * @param[in] cpupro 卡协议编号，0x00表示T0，0x01表示T1，默认为0x00。
   * @param[in] cpuetu 卡复位波特率编号，0x5C表示9600，0x14表示38400，0x0E表示115200。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_setcpupara(HANDLE icdev, unsigned char cputype, unsigned char cpupro, unsigned char cpuetu);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_command(HANDLE icdev, unsigned char cmd, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_command_hex(HANDLE icdev, unsigned char cmd, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_creat_mac(unsigned char KeyLen, unsigned char *Key, unsigned short DataLen, unsigned char *Data, unsigned char *InitData, unsigned char AutoFixFlag, unsigned char FixChar, unsigned char *MacData);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_creat_mac_hex(unsigned char KeyLen, unsigned char *Key, unsigned short DataLen, unsigned char *Data, unsigned char *InitData, unsigned char AutoFixFlag, unsigned char FixChar, unsigned char *MacData);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_creat_mac2(HANDLE icdev, unsigned short DataLen, unsigned char *Data, unsigned char *MacData, unsigned char flag);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_creat_mac2_hex(HANDLE icdev, unsigned short DataLen, unsigned char *Data, unsigned char *MacData, unsigned char flag);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_encrypt(char *key, char *ptrSource, unsigned short msgLen, char *ptrDest);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_decrypt(char *key, char *ptrSource, unsigned short msgLen, char *ptrDest);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_encrypt_hex(char *key, char *ptrSource, unsigned short msgLen, char *ptrDest);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_decrypt_hex(char *key, char *ptrSource, unsigned short msgLen, char *ptrDest);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_HL_write_hex(HANDLE icdev, unsigned char _Mode, unsigned char _Adr, unsigned int *_Snr, unsigned char *_Data);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_HL_read_hex(HANDLE icdev, unsigned char _Mode, unsigned char _Adr, unsigned int _Snr, unsigned char *_Data, unsigned int *_NSnr);

  /**
   * @brief  检查之前写入的数据。
   * @par    说明：
   * ::dc_check_write 的HEX形式接口，参数 @a _data 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_check_write_hex(HANDLE icdev, unsigned int Snr, unsigned char authmode, unsigned char Adr, unsigned char *_data);

  /**
   * @brief  读EEPROM。
   * @par    说明：
   * ::dc_srd_eeprom 的HEX形式接口，参数 @a rec_buffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_srd_eeprom_hex(HANDLE icdev, short offset, short length, unsigned char *rec_buffer);

  /**
   * @brief  写EEPROM。
   * @par    说明：
   * ::dc_swr_eeprom 的HEX形式接口，参数 @a send_buffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_swr_eeprom_hex(HANDLE icdev, short offset, short length, unsigned char *send_buffer);

  /**
   * @brief  获取设备时间。
   * @par    说明：
   * ::dc_gettime 的HEX形式接口，参数 @a time 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_gettime_hex(HANDLE icdev, char *time);

  /**
   * @brief  设置设备时间。
   * @par    说明：
   * ::dc_settime 的HEX形式接口，参数 @a time 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_settime_hex(HANDLE icdev, char *time);

  /**
   * @brief  单DES加解密。
   * @par    说明：
   * ::dc_des 的HEX形式接口，参数 @a key @a sour @a dest 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_des_hex(unsigned char *key, unsigned char *sour, unsigned char *dest, short m);

  /**
   * @brief  单DES加解密。
   * @par    说明：
   * 使用单DES算法对数据进行加密/解密。
   * @param[in] key 8个字节密钥。
   * @param[in] sour 8个字节要做加密/解密的数据。
   * @param[out] dest 返回8个字节运算后的数据。
   * @param[in] m 模式，0-解密，1-加密。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_des(unsigned char *key, unsigned char *sour, unsigned char *dest, short m);

  /**
   * @brief  非接触式CPU卡复位。
   * @par    说明：
   * ::dc_pro_reset 的HEX形式接口，参数 @a receive_data 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_pro_reset_hex(HANDLE icdev, unsigned char *rlen, char *receive_data);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * ::dc_pro_command 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_pro_command_hex(HANDLE icdev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer, unsigned char timeout);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * ::dc_pro_commandsource 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_pro_commandsource_hex(HANDLE icdev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer, unsigned char timeout);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_switch_unix(HANDLE icdev, int baud);

  /**
   * @brief  验证M1卡密码。
   * @par    说明：
   * 使用传入的密码来验证M1卡密码。
   * @param[in] icdev 设备标识符。
   * @param[in] _Mode 模式，0x00表示验证A密码，0x04表示验证B密码。
   * @param[in] _Addr 要验证密码的块号。
   * @param[in] passbuff 密码，固定为6个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_authentication_passaddr(HANDLE icdev, unsigned char _Mode, unsigned char _Addr, unsigned char *passbuff);

  /**
   * @brief  验证M1卡密码。
   * @par    说明：
   * ::dc_authentication_passaddr 的HEX形式接口，参数 @a passbuff 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_authentication_passaddr_hex(HANDLE icdev, unsigned char _Mode, unsigned char _Addr, unsigned char *passbuff);

  /**
   * @brief  寻FM11RF005卡。
   * @par    说明：
   * 对FM11RF005卡进行寻卡操作。
   * @param[in] icdev 设备标识符。
   * @param[in] _Mode 模式，模式，同 ::dc_request 的 @a _Mode 。
   * @param[out] _Snr 返回的卡序列号。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  API_ATTRIBUTE short USER_API dc_card_fm11rf005(HANDLE icdev, unsigned char _Mode, unsigned int *_Snr);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_setusbtimeout(unsigned char ntimes);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_mfdes_baud(HANDLE icdev, unsigned char _Mode, unsigned char para);

  /**
   * @brief  三DES加解密。
   * @par    说明：
   * 使用三DES算法对数据进行加密/解密。
   * @param[in] key 16个字节密钥。
   * @param[in] src 8个字节要做加密/解密的数据。
   * @param[out] dest 返回8个字节运算后的数据。
   * @param[in] m 模式，0-解密，1-加密。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_tripledes(unsigned char *key, unsigned char *src, unsigned char *dest, short m);

  /**
   * @brief  三DES加解密。
   * @par    说明：
   * ::dc_tripledes 的HEX形式接口，参数 @a key @a src @a dest 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_tripledes_hex(unsigned char *key, unsigned char *src, unsigned char *dest, short m);

  /**
   * @brief  Mifare Desfire卡认证。
   * @par    说明：
   * ::dc_mfdes_auth 的HEX形式接口，参数 @a authkey @a randAdata @a randBdata 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_mfdes_auth_hex(HANDLE icdev, unsigned char keyno, unsigned char keylen, unsigned char *authkey, unsigned char *randAdata, unsigned char *randBdata);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_pro_sendcommandsource(HANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char timeout);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_pro_receivecommandsource(HANDLE icdev, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  15693卡寻卡请求。
   * @par    说明：
   * 支持15693卡的寻卡请求。
   * @param[in] icdev 设备标识符。
   * @param[in] flags 标志，0x32表示寻单张卡，0x16表示寻多张卡。
   * @param[in] AFI 应用族识别符。
   * @param[in] masklen 掩码长度。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rbuffer 返回的数据，请至少分配256个字节。格式为：【1字节DSFID1】【8字节UID1】【1字节DSFID2】【8字节UID2】...。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_inventory(HANDLE icdev, unsigned char flags, unsigned char AFI, unsigned char masklen, unsigned char *rlen, unsigned char *rbuffer);

  /**
   * @brief  15693卡寻卡请求。
   * @par    说明：
   * ::dc_inventory 的HEX形式接口，参数 @a rbuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_inventory_hex(HANDLE icdev, unsigned char flags, unsigned char AFI, unsigned char masklen, unsigned char *rlen, unsigned char *rbuffer);

  /**
   * @brief  15693卡静默。
   * @par    说明：
   * 使指定的15693卡进入静默状态。
   * @param[in] icdev 设备标识符。
   * @param[in] flags 标志，固定为0x22。
   * @param[in] UID 卡片UID，固定为8个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_stay_quiet(HANDLE icdev, unsigned char flags, unsigned char *UID);

  /**
   * @brief  15693卡静默。
   * @par    说明：
   * ::dc_stay_quiet 的HEX形式接口，参数 @a UID 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_stay_quiet_hex(HANDLE icdev, unsigned char flags, unsigned char *UID);

  /**
   * @brief  15693卡选择。
   * @par    说明：
   * 使指定的15693卡进入选择状态。
   * @param[in] icdev 设备标识符。
   * @param[in] flags 标志，固定为0x22。
   * @param[in] UID 卡片UID，固定为8个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_select_uid(HANDLE icdev, unsigned char flags, unsigned char *UID);

  /**
   * @brief  15693卡选择。
   * @par    说明：
   * ::dc_select_uid 的HEX形式接口，参数 @a UID 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_select_uid_hex(HANDLE icdev, unsigned char flags, unsigned char *UID);

  /**
   * @brief  15693卡准备。
   * @par    说明：
   * 使指定的15693卡进入准备状态。
   * @param[in] icdev 设备标识符。
   * @param[in] flags 标志，固定为0x22。
   * @param[in] UID 卡片UID，固定为8个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_reset_to_ready(HANDLE icdev, unsigned char flags, unsigned char *UID);

  /**
   * @brief  15693卡准备。
   * @par    说明：
   * ::dc_reset_to_ready 的HEX形式接口，参数 @a UID 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_reset_to_ready_hex(HANDLE icdev, unsigned char flags, unsigned char *UID);

  /**
   * @brief  15693卡读块数据。
   * @par    说明：
   * 读取15693卡的块数据。
   * @param[in] icdev 设备标识符。
   * @param[in] flags 标志，固定为0x22。
   * @param[in] startblock 起始块地址，从0x00开始编号。
   * @param[in] blocknum 块数量，范围1~10。
   * @param[in] UID 卡片UID，固定为8个字节。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rbuffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_readblock(HANDLE icdev, unsigned char flags, unsigned char startblock, unsigned char blocknum, unsigned char *UID, unsigned char *rlen, unsigned char *rbuffer);

  /**
   * @brief  15693卡读块数据。
   * @par    说明：
   * ::dc_readblock 的HEX形式接口，参数 @a UID @a rbuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_readblock_hex(HANDLE icdev, unsigned char flags, unsigned char startblock, unsigned char blocknum, unsigned char *UID, unsigned char *rlen, unsigned char *rbuffer);

  /**
   * @brief  15693卡写块数据。
   * @par    说明：
   * 写入块数据到15693卡中。
   * @param[in] icdev 设备标识符。
   * @param[in] flags 标志，ICode2卡为0x22，Ti2K卡为0xC3。
   * @param[in] startblock 起始块地址，从0x00开始编号。
   * @param[in] blocknum 块数量，范围1~10。
   * @param[in] UID 卡片UID，固定为8个字节。
   * @param[in] wlen 写入数据的长度。
   * @param[in] wbuffer 写入数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_writeblock(HANDLE icdev, unsigned char flags, unsigned char startblock, unsigned char blocknum, unsigned char *UID, unsigned char wlen, unsigned char *wbuffer);

  /**
   * @brief  15693卡写块数据。
   * @par    说明：
   * ::dc_writeblock 的HEX形式接口，参数 @a UID @a wbuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_writeblock_hex(HANDLE icdev, unsigned char flags, unsigned char startblock, unsigned char blocknum, unsigned char *UID, unsigned char wlen, unsigned char *wbuffer);

  /**
   * @brief  15693卡锁块。
   * @par    说明：
   * 使指定的15693卡块数据被锁定。
   * @param[in] icdev 设备标识符。
   * @param[in] flags 标志，ICode2卡为0x22，Ti2K卡为0xC3。
   * @param[in] block 块地址，从0x00开始编号。
   * @param[in] UID 卡片UID，固定为8个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_lock_block(HANDLE icdev, unsigned char flags, unsigned char block, unsigned char *UID);

  /**
   * @brief  15693卡锁块。
   * @par    说明：
   * ::dc_lock_block 的HEX形式接口，参数 @a UID 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_lock_block_hex(HANDLE icdev, unsigned char flags, unsigned char block, unsigned char *UID);

  /**
   * @brief  15693卡写AFI。
   * @par    说明：
   * 15693卡写AFI。
   * @param[in] icdev 设备标识符。
   * @param[in] flags 标志，ICode2卡为0x22，Ti2K卡为0xC3。
   * @param[in] AFI 应用族识别符。
   * @param[in] UID 卡片UID，固定为8个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_write_afi(HANDLE icdev, unsigned char flags, unsigned char AFI, unsigned char *UID);

  /**
   * @brief  15693卡写AFI。
   * @par    说明：
   * ::dc_write_afi 的HEX形式接口，参数 @a UID 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_write_afi_hex(HANDLE icdev, unsigned char flags, unsigned char AFI, unsigned char *UID);

  /**
   * @brief  15693卡锁AFI。
   * @par    说明：
   * 15693卡锁AFI。
   * @param[in] icdev 设备标识符。
   * @param[in] flags 标志，ICode2卡为0x22，Ti2K卡为0xC3。
   * @param[in] AFI 应用族识别符。
   * @param[in] UID 卡片UID，固定为8个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_lock_afi(HANDLE icdev, unsigned char flags, unsigned char AFI, unsigned char *UID);

  /**
   * @brief  15693卡锁AFI。
   * @par    说明：
   * ::dc_lock_afi 的HEX形式接口，参数 @a UID 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_lock_afi_hex(HANDLE icdev, unsigned char flags, unsigned char AFI, unsigned char *UID);

  /**
   * @brief  15693卡写DSFID。
   * @par    说明：
   * 15693卡写DSFID。
   * @param[in] icdev 设备标识符。
   * @param[in] flags 标志，ICode2卡为0x22，Ti2K卡为0xC3。
   * @param[in] DSFID 数据存储格式识别符。
   * @param[in] UID 卡片UID，固定为8个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_write_dsfid(HANDLE icdev, unsigned char flags, unsigned char DSFID, unsigned char *UID);

  /**
   * @brief  15693卡写DSFID。
   * @par    说明：
   * ::dc_write_dsfid 的HEX形式接口，参数 @a UID 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_write_dsfid_hex(HANDLE icdev, unsigned char flags, unsigned char DSFID, unsigned char *UID);

  /**
   * @brief  15693卡锁DSFID。
   * @par    说明：
   * 15693卡锁DSFID。
   * @param[in] icdev 设备标识符。
   * @param[in] flags 标志，ICode2卡为0x22，Ti2K卡为0xC3。
   * @param[in] DSFID 数据存储格式识别符。
   * @param[in] UID 卡片UID，固定为8个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_lock_dsfid(HANDLE icdev, unsigned char flags, unsigned char DSFID, unsigned char *UID);

  /**
   * @brief  15693卡锁DSFID。
   * @par    说明：
   * ::dc_lock_dsfid 的HEX形式接口，参数 @a UID 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_lock_dsfid_hex(HANDLE icdev, unsigned char flags, unsigned char DSFID, unsigned char *UID);

  /**
   * @brief  获取15693卡系统信息。
   * @par    说明：
   * 获取15693卡系统信息。
   * @param[in] icdev 设备标识符。
   * @param[in] flags 标志，固定为0x22。
   * @param[in] UID 卡片UID，固定为8个字节。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rbuffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_get_systeminfo(HANDLE icdev, unsigned char flags, unsigned char *UID, unsigned char *rlen, unsigned char *rbuffer);

  /**
   * @brief  获取15693卡系统信息。
   * @par    说明：
   * ::dc_get_systeminfo 的HEX形式接口，参数 @a UID @a rbuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_get_systeminfo_hex(HANDLE icdev, unsigned char flags, unsigned char *UID, unsigned char *rlen, unsigned char *rbuffer);

  /**
   * @brief  获取15693卡安全信息。
   * @par    说明：
   * 获取15693卡安全信息。
   * @param[in] icdev 设备标识符。
   * @param[in] flags 标志，固定为0x22。
   * @param[in] startblock 起始块地址，从0x00开始编号。
   * @param[in] blocknum 块数量，范围1~10。
   * @param[in] UID 卡片UID，固定为8个字节。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rbuffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_get_securityinfo(HANDLE icdev, unsigned char flags, unsigned char startblock, unsigned char blocknum, unsigned char *UID, unsigned char *rlen, unsigned char *rbuffer);

  /**
   * @brief  获取15693卡安全信息。
   * @par    说明：
   * ::dc_get_securityinfo 的HEX形式接口，参数 @a UID @a rbuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_get_securityinfo_hex(HANDLE icdev, unsigned char flags, unsigned char startblock, unsigned char blocknum, unsigned char *UID, unsigned char *rlen, unsigned char *rbuffer);

  /**
   * @brief  获取FM11RF005卡序列号。
   * @par    说明：
   * 获取FM11RF005卡序列号。
   * @param[in] icdev 设备标识符。
   * @param[out] _Snr 返回的卡序列号。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_getsnr_fm11rf005(HANDLE icdev, unsigned int *_Snr);

  /**
   * @brief  获取FM11RF005卡序列号。
   * @par    说明：
   * ::dc_getsnr_fm11rf005 的HEX形式接口，参数 @a snrstr 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_getsnr_fm11rf005_hex(HANDLE icdev, unsigned char *snrstr);

  /**
   * @brief  写FM11RF005卡。
   * @par    说明：
   * 写入数据到FM11RF005卡中。
   * @param[in] icdev 设备标识符。
   * @param[in] _Adr 块地址（2~15）。
   * @param[in] _Data 传入数据，固定为4个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_write_fm11rf005(HANDLE icdev, unsigned char _Adr, unsigned char *_Data);

  /**
   * @brief  读FM11RF005卡。
   * @par    说明：
   * 读取FM11RF005卡的数据。
   * @param[in] icdev 设备标识符。
   * @param[in] _Adr 块地址（0~15）。
   * @param[out] _Data 返回的数据，固定为4个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_read_fm11rf005(HANDLE icdev, unsigned char _Adr, unsigned char *_Data);

  /**
   * @brief  读FM11RF005卡。
   * @par    说明：
   * ::dc_read_fm11rf005 的HEX形式接口，参数 @a _Data 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_read_fm11rf005_hex(HANDLE icdev, unsigned char _Adr, char *_Data);

  /**
   * @brief  写FM11RF005卡。
   * @par    说明：
   * ::dc_write_fm11rf005 的HEX形式接口，参数 @a _Data 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_write_fm11rf005_hex(HANDLE icdev, unsigned char _Adr, char *_Data);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API DCDEV_CommandMcu(HANDLE icdev, unsigned char ctimeout, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API DCDEV_CommandMcu_Hex(HANDLE icdev, unsigned char ctimeout, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  显示LCD。
   * @par    说明：
   * 显示LCD。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 标志。
   * @n 0x00 - 关背光。
   * @n 0x01 - 开背光。
   * @n 0x10 - 清屏。
   * @n 0x20 - 欢迎使用。
   * @n 0x21 - 请输入密码。
   * @n 0x22 - 请再次输入密码。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_displcd(HANDLE icdev, unsigned char flag);

  /**
   * @brief  键盘密码输入。
   * @par    说明：
   * 获取键盘按键，用于密码输入，不同的键盘装置可能有不同的效果，如不同的提示音或不同的显示。
   * @param[in] icdev 设备标识符。
   * @param[in] ctime 设备超时值，单位为秒。
   * @param[out] rlen 返回按键数据的长度，不含'\0'。
   * @param[out] cpass 返回的按键数据，数据格式为'\0'结尾的字符串。
   * @return <0表示失败，==0表示成功，==0xA1表示取消输入，==0xA2表示输入超时。
   */
  API_ATTRIBUTE short USER_API dc_getinputpass(HANDLE icdev, unsigned char ctime, unsigned char *rlen, unsigned char *cpass);

  /**
   * @brief  键盘密码输入。
   * @par    说明：
   * 获取键盘按键，用于密码输入，不同的键盘装置可能有不同的效果，如不同的提示音或不同的显示。
   * @param[in] icdev 设备标识符。
   * @param[in] ctime 设备超时值，单位为秒。
   * @param[out] rlen 返回按键数据的长度，不含'\0'。
   * @param[out] cpass 返回的按键数据，数据格式为'\0'结尾的字符串。
   * @return <0表示失败，==0表示成功，==0xA1表示取消输入，==0xA2表示输入超时。
   */
  API_ATTRIBUTE short USER_API dc_GetInputPass(HANDLE icdev, unsigned char ctime, unsigned char *rlen, unsigned char *cpass);

  /**
   * @brief  读磁条卡。
   * @par    说明：
   * 读取磁条卡的数据，支持2、3轨道。
   * @param[in] icdev 设备标识符。
   * @param[in] ctime 设备超时值，单位为秒。
   * @param[out] pTrack2Data 返回的2轨道数据，数据格式为'\0'结尾的字符串。
   * @param[out] pTrack2Len 返回2轨道数据的长度，不含'\0'。
   * @param[out] pTrack3Data 返回的3轨道数据，数据格式为'\0'结尾的字符串。
   * @param[out] pTrack3Len 返回3轨道数据的长度，不含'\0'。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_readmagcard(HANDLE icdev, unsigned char ctime, unsigned char *pTrack2Data, unsigned int *pTrack2Len, unsigned char *pTrack3Data, unsigned int *pTrack3Len);

  /**
   * @brief  写磁条卡。
   * @par    说明：
   * 写入数据到磁条卡中。
   * @param[in] icdev 设备标识符。
   * @param[in] ctime 设备超时值，单位为秒。
   * @param[in] pTrack1Data 传入1轨道数据。
   * @param[in] pTrack1Len 1轨道数据的长度。
   * @param[in] pTrack2Data 传入2轨道数据。
   * @param[in] pTrack2Len 2轨道数据的长度。
   * @param[in] pTrack3Data 传入3轨道数据。
   * @param[in] pTrack3Len 3轨道数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_writemagcard(HANDLE icdev, unsigned char ctime, unsigned char *pTrack1Data, unsigned char pTrack1Len, unsigned char *pTrack2Data, unsigned char pTrack2Len, unsigned char *pTrack3Data, unsigned char pTrack3Len);

  /**
   * @brief  测试设备通讯。
   * @par    说明：
   * 测试和设备之间是否可以正常通讯。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_testdevicecomm(HANDLE icdev);

  /**
   * @brief  显示主界面。
   * @par    说明：
   * 使设备屏幕上显示预置的主界面，不同设备的主界面不尽相同。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_dispmainmenu(HANDLE icdev);

  /**
   * @brief  设置设备时间。
   * @par    说明：
   * 设置设备的当前时间。
   * @param[in] icdev 设备标识符。
   * @param[in] year 年（0~99）。
   * @param[in] month 月（1~12）。
   * @param[in] date 日（1~31）。
   * @param[in] hour 时（0~23）。
   * @param[in] minute 分（0~59）。
   * @param[in] second 秒（0~59）。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_setdevicetime(HANDLE icdev, unsigned char year, unsigned char month, unsigned char date, unsigned char hour, unsigned char minute, unsigned char second);

  /**
   * @brief  获取设备时间。
   * @par    说明：
   * 获取设备当前的时间。
   * @param[in] icdev 设备标识符。
   * @param[out] year 年（0~99）。
   * @param[out] month 月（1~12）。
   * @param[out] date 日（1~31）。
   * @param[out] hour 时（0~23）。
   * @param[out] minute 分（0~59）。
   * @param[out] second 秒（0~59）。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_getdevicetime(HANDLE icdev, unsigned char *year, unsigned char *month, unsigned char *date, unsigned char *hour, unsigned char *minute, unsigned char *second);

  /**
   * @brief  液晶显示。
   * @par    说明：
   * 显示指定字符串到液晶屏幕上。
   * @param[in] icdev 设备标识符。
   * @param[in] line 行号。
   * @param[in] offset 偏移。
   * @param[in] data 要显示的字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_dispinfo(HANDLE icdev, unsigned char line, unsigned char offset, char *data);

  /**
   * @brief  液晶显示。
   * @par    说明：
   * 显示指定字符串到液晶屏幕上。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 屏幕偏移。
   * @param[in] data 要显示的字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_dispmaininfo(HANDLE icdev, unsigned char offset, char *data);

  /**
   * @brief  设备蜂鸣。
   * @par    说明：
   * 设备蜂鸣器发出指定时间的蜂鸣声。
   * @param[in] icdev 设备标识符。
   * @param[in] beeptime 蜂鸣时间，单位为50毫秒。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_posbeep(HANDLE icdev, unsigned char beeptime);

  /**
   * @brief  LCD背光控制。
   * @par    说明：
   * 控制设备的LCD背光。
   * @param[in] icdev 设备标识符。
   * @param[in] cOpenFlag 0-开背光，1-关背光。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ctlbacklight(HANDLE icdev, unsigned char cOpenFlag);

  /**
   * @brief  指示灯控制。
   * @par    说明：
   * 控制设备的指示灯。
   * @param[in] icdev 设备标识符。
   * @param[in] cLed 指示灯编号，0表示全部指示灯，1表示第一个指示灯，2表示第二个指示灯，以此类推。
   * @param[in] cOpenFlag 0-点亮，1-熄灭，2-闪烁。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ctlled(HANDLE icdev, unsigned char cLed, unsigned char cOpenFlag);

  /**
   * @brief  LCD清屏。
   * @par    说明：
   * 清除LCD屏幕的显示内容。
   * @param[in] icdev 设备标识符。
   * @param[in] cLine 需要清除的行号，编号从1开始。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_lcdclrscrn(HANDLE icdev, unsigned char cLine);

  /**
   * @brief  进入读键盘模式。
   * @par    说明：
   * 使设备进入读取键盘的状态，接口本身会直接返回，而不会等待按键完成或设备超时。
   * @param[in] icdev 设备标识符。
   * @param[in] ctime 设备超时值，单位为秒。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_passin(HANDLE icdev, unsigned char ctime);

  /**
   * @brief  进入读键盘模式。
   * @par    说明：
   * 使设备进入读取键盘的状态，接口本身会直接返回，而不会等待按键完成或设备超时。
   * @param[in] icdev 设备标识符。
   * @param[in] ctime 设备超时值，单位为秒。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_PassIn(HANDLE icdev, unsigned char ctime);

  /**
   * @brief  进入读键盘模式。
   * @par    说明：
   * 使设备进入读取键盘的状态，接口本身会直接返回，而不会等待按键完成或设备超时。
   * @param[in] icdev 设备标识符。
   * @param[in] ctime 设备超时值，单位为秒。
   * @param[in] ucmode 模式，0-带卡号，1-不带卡号。
   * @param[in] cardno 卡号字符串，只用于带卡号的模式。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_passin2(HANDLE icdev, unsigned char ctime, unsigned char ucmode, unsigned char *cardno);

  /**
   * @brief  进入读键盘模式。
   * @par    说明：
   * 使设备进入读取键盘的状态，接口本身会直接返回，而不会等待按键完成或设备超时。
   * @param[in] icdev 设备标识符。
   * @param[in] ctime 设备超时值，单位为秒。
   * @param[in] ucmode 模式，0-带卡号，1-不带卡号。
   * @param[in] cardno 卡号字符串，只用于带卡号的模式。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_PassIn2(HANDLE icdev, unsigned char ctime, unsigned char ucmode, unsigned char *cardno);

  /**
   * @brief  获取键盘数据。
   * @par    说明：
   * 获取设备已经读取到的键盘数据。
   * @param[in] icdev 设备标识符。
   * @param[out] rlen 返回按键数据的长度，不含'\0'。
   * @param[out] cpass 返回的按键数据，数据格式为'\0'结尾的字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_passget(HANDLE icdev, unsigned char *rlen, unsigned char *cpass);

  /**
   * @brief  获取键盘数据。
   * @par    说明：
   * 获取设备已经读取到的键盘数据。
   * @param[in] icdev 设备标识符。
   * @param[out] rlen 返回按键数据的长度，不含'\0'。
   * @param[out] cpass 返回的按键数据，数据格式为'\0'结尾的字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_PassGet(HANDLE icdev, unsigned char *rlen, unsigned char *cpass);

  /**
   * @brief  退出读键盘模式。
   * @par    说明：
   * 在设备进入读键盘模式后，设备会一直处于接收键盘输入的状态，这时只有按键完成、设备超时或调用此接口才能使得设备退出这种状态。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_passcancel(HANDLE icdev);

  /**
   * @brief  退出读键盘模式。
   * @par    说明：
   * 在设备进入读键盘模式后，设备会一直处于接收键盘输入的状态，这时是只有按键完成、设备超时或调用此接口才能使得设备退出这种状态。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_PassCancel(HANDLE icdev);

  /**
   * @brief  获取键盘输入。
   * @par    说明：
   * 获取键盘按键，同时设置输入时屏幕显示的信息。
   * @param[in] icdev 设备标识符。
   * @param[in] disptype 显示类型，此类型与设备内置显示条目相关。
   * @param[in] line 行号，信息将会显示在此行号指定的行中。
   * @param[in] ctime 设备超时值，单位为秒。
   * @param[out] rlen 返回按键数据的长度，不含'\0'。
   * @param[out] ckeydata 返回的按键数据，数据格式为'\0'结尾的字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_getinputkey(HANDLE icdev, unsigned char disptype, unsigned char line, unsigned char ctime, unsigned char *rlen, unsigned char *ckeydata);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_displcd_ext(HANDLE icdev, unsigned char flag, unsigned char row, unsigned char offset);

  /**
   * @brief  读磁条卡。
   * @par    说明：
   * 读取磁条卡的数据，支持1、2、3轨道。
   * @param[in] icdev 设备标识符。
   * @param[in] ctime 设备超时值，单位为秒。
   * @param[out] pTrack1Data 返回的1轨道数据，数据格式为'\0'结尾的字符串。
   * @param[out] pTrack1Len 返回1轨道数据的长度，不含'\0'。
   * @param[out] pTrack2Data 返回的2轨道数据，数据格式为'\0'结尾的字符串。
   * @param[out] pTrack2Len 返回2轨道数据的长度，不含'\0'。
   * @param[out] pTrack3Data 返回的3轨道数据，数据格式为'\0'结尾的字符串。
   * @param[out] pTrack3Len 返回3轨道数据的长度，不含'\0'。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_readmagcardall(HANDLE icdev, unsigned char ctime, unsigned char *pTrack1Data, unsigned int *pTrack1Len, unsigned char *pTrack2Data, unsigned int *pTrack2Len, unsigned char *pTrack3Data, unsigned int *pTrack3Len);

  /**
   * @brief  获取设备序列号。
   * @par    说明：
   * 获取设备内部定制的序列号，设备默认序列号为空，只有预先定制的设备才会存在可用的序列号。
   * @param[in] icdev 设备标识符。
   * @param[out] snr 返回的序列号字符串，请至少分配33个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_readdevsnr(HANDLE icdev, unsigned char *snr);

  /**
   * @brief  获取设备序列号。
   * @par    说明：
   * 获取设备内部定制的序列号，设备默认序列号为空，只有预先定制的设备才会存在可用的序列号。
   * @param[in] icdev 设备标识符。
   * @param[out] snr 返回的序列号字符串，请至少分配17个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_readreadersnr(HANDLE icdev, unsigned char *snr);

  /**
   * @brief  复位设备。
   * @par    说明：
   * 使设备进入上电初始状态。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_resetdevice(HANDLE icdev);

  /**
   * @brief  设置串口设备通讯波特率。
   * @par    说明：
   * 设置串口设备通讯波特率。
   * @param[in] icdev 设备标识符。
   * @param[in] baud 波特率。
   * @n 0x00 - 115200。
   * @n 0x01 - 57600。
   * @n 0x02 - 38400。
   * @n 0x03 - 19200。
   * @n 0x04 - 9600。
   * @n 0x05 - 4800。
   * @n 0x06 - 2400。
   * @n 0x07 - 1200。
   * @n 0x08 - 460800。
   * @n 0x09 - 128000。
   * @n 0x0A - 230400。
   * @n 0x0B - 256000。
   * @n 0x0C - 921600。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_setdevicebaud(HANDLE icdev, unsigned char baud);

  /**
   * @brief  读4442卡。
   * @par    说明：
   * 读取4442卡的数据。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 读取长度。
   * @param[out] data_buffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_read_4442(HANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  读4442卡。
   * @par    说明：
   * ::dc_read_4442 的HEX形式接口，参数 @a data_buffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_read_4442_hex(HANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  写4442卡。
   * @par    说明：
   * 写入数据到4442卡中。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 写入长度。
   * @param[in] data_buffer 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_write_4442(HANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  写4442卡。
   * @par    说明：
   * ::dc_write_4442 的HEX形式接口，参数 @a data_buffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_write_4442_hex(HANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  验证4442卡密码。
   * @par    说明：
   * 使用传入的密码来验证4442卡密码。
   * @param[in] icdev 设备标识符。
   * @param[in] passwd 密码，固定为3个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_verifypin_4442(HANDLE icdev, unsigned char *passwd);

  /**
   * @brief  验证4442卡密码。
   * @par    说明：
   * ::dc_verifypin_4442 的HEX形式接口，参数 @a passwd 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_verifypin_4442_hex(HANDLE icdev, unsigned char *passwd);

  /**
   * @brief  读4442卡密码。
   * @par    说明：
   * 读取4442卡的密码。
   * @param[in] icdev 设备标识符。
   * @param[out] passwd 密码，固定为3个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_readpin_4442(HANDLE icdev, unsigned char *passwd);

  /**
   * @brief  读4442卡密码。
   * @par    说明：
   * ::dc_readpin_4442 的HEX形式接口，参数 @a passwd 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_readpin_4442_hex(HANDLE icdev, unsigned char *passwd);

  /**
   * @brief  读4442卡密码计数。
   * @par    说明：
   * 读取4442卡的密码计数，此计数值表示可以尝试验证密码的次数。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，否则为密码计数值。
   */
  API_ATTRIBUTE short USER_API dc_readpincount_4442(HANDLE icdev);

  /**
   * @brief  修改4442卡密码。
   * @par    说明：
   * 修改4442卡的密码。
   * @param[in] icdev 设备标识符。
   * @param[in] passwd 密码，固定为3个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_changepin_4442(HANDLE icdev, unsigned char *passwd);

  /**
   * @brief  修改4442卡密码。
   * @par    说明：
   * ::dc_changepin_4442 的HEX形式接口，参数 @a passwd 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_changepin_4442_hex(HANDLE icdev, unsigned char *passwd);

  /**
   * @brief  读4442卡保护位。
   * @par    说明：
   * 读取4442卡的保护区中哪些位置已经被置保护。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 读取长度。
   * @param[out] data_buffer 返回的数据，数据中含有0x00字节的位置表示已经被置保护。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_readwrotect_4442(HANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  读4442卡保护位。
   * @par    说明：
   * ::dc_readwrotect_4442 的HEX形式接口，参数 @a data_buffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_readwrotect_4442_hex(HANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  读4442卡保护位。
   * @par    说明：
   * 读取4442卡的保护区中哪些位置已经被置保护。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 读取长度。
   * @param[out] data_buffer 返回的数据，数据中含有0x00字节的位置表示已经被置保护。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_readprotect_4442(HANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  读4442卡保护位。
   * @par    说明：
   * ::dc_readprotect_4442 的HEX形式接口，参数 @a data_buffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_readprotect_4442_hex(HANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  写4442卡保护位。
   * @par    说明：
   * 对4442卡的保护区中指定位置进行置保护。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 写入长度。
   * @param[in] data_buffer 传入数据，数据中和卡内原有数据相同的字节位置将被置保护。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_writeprotect_4442(HANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  写4442卡保护位。
   * @par    说明：
   * ::dc_writeprotect_4442 的HEX形式接口，参数 @a data_buffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_writeprotect_4442_hex(HANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  写24C系列卡。
   * @par    说明：
   * 写入数据到24C系列卡中，支持24C01、24C02、24C04、24C08、24C16、24C32卡。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 写入长度。
   * @param[in] snd_buffer 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_write_24c(HANDLE icdev, short offset, short length, unsigned char *snd_buffer);

  /**
   * @brief  写24C系列卡。
   * @par    说明：
   * ::dc_write_24c 的HEX形式接口，参数 @a snd_buffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_write_24c_hex(HANDLE icdev, short offset, short length, unsigned char *snd_buffer);

  /**
   * @brief  写24C64系列卡。
   * @par    说明：
   * 写入数据到24C64系列卡中，支持24C64、24C128、24C256、24C512、24C1024、24C2048、24C4096卡。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 写入长度。
   * @param[in] snd_buffer 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_write_24c64(HANDLE icdev, short offset, short length, unsigned char *snd_buffer);

  /**
   * @brief  写24C64系列卡。
   * @par    说明：
   * ::dc_write_24c64 的HEX形式接口，参数 @a snd_buffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_write_24c64_hex(HANDLE icdev, short offset, short length, unsigned char *snd_buffer);

  /**
   * @brief  读24C系列卡。
   * @par    说明：
   * 读取24C系列卡的数据，支持24C01、24C02、24C04、24C08、24C16、24C32卡。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 读取长度。
   * @param[out] receive_buffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_read_24c(HANDLE icdev, short offset, short length, unsigned char *receive_buffer);

  /**
   * @brief  读24C系列卡。
   * @par    说明：
   * ::dc_read_24c 的HEX形式接口，参数 @a receive_buffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_read_24c_hex(HANDLE icdev, short offset, short length, unsigned char *receive_buffer);

  /**
   * @brief  读24C64系列卡。
   * @par    说明：
   * 读取24C64系列卡的数据，支持24C64、24C128、24C256、24C512、24C1024、24C2048、24C4096卡。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 读取长度。
   * @param[out] receive_buffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_read_24c64(HANDLE icdev, short offset, short length, unsigned char *receive_buffer);

  /**
   * @brief  读24C64系列卡。
   * @par    说明：
   * ::dc_read_24c64 的HEX形式接口，参数 @a receive_buffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_read_24c64_hex(HANDLE icdev, short offset, short length, unsigned char *receive_buffer);

  /**
   * @brief  读4428卡。
   * @par    说明：
   * 读取4428卡的数据。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 读取长度。
   * @param[out] data_buffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_read_4428(HANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  读4428卡。
   * @par    说明：
   * ::dc_read_4428 的HEX形式接口，参数 @a data_buffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_read_4428_hex(HANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  写4428卡。
   * @par    说明：
   * 写入数据到4428卡中。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 写入长度。
   * @param[in] data_buffer 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_write_4428(HANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  写4428卡。
   * @par    说明：
   * ::dc_write_4428 的HEX形式接口，参数 @a data_buffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_write_4428_hex(HANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  验证4428卡密码。
   * @par    说明：
   * 使用传入的密码来验证4428卡密码。
   * @param[in] icdev 设备标识符。
   * @param[in] passwd 密码，固定为2个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_verifypin_4428(HANDLE icdev, unsigned char *passwd);

  /**
   * @brief  验证4428卡密码。
   * @par    说明：
   * ::dc_verifypin_4428 的HEX形式接口，参数 @a passwd 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_verifypin_4428_hex(HANDLE icdev, unsigned char *passwd);

  /**
   * @brief  读4428卡密码。
   * @par    说明：
   * 读取4428卡的密码。
   * @param[in] icdev 设备标识符。
   * @param[out] passwd 密码，固定为2个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_readpin_4428(HANDLE icdev, unsigned char *passwd);

  /**
   * @brief  读4428卡密码。
   * @par    说明：
   * ::dc_readpin_4428 的HEX形式接口，参数 @a passwd 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_readpin_4428_hex(HANDLE icdev, unsigned char *passwd);

  /**
   * @brief  读4428卡密码计数。
   * @par    说明：
   * 读取4428卡的密码计数，此计数值表示可以尝试验证密码的次数。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，否则为密码计数值。
   */
  API_ATTRIBUTE short USER_API dc_readpincount_4428(HANDLE icdev);

  /**
   * @brief  修改4428卡密码。
   * @par    说明：
   * 修改4428卡的密码。
   * @param[in] icdev 设备标识符。
   * @param[in] passwd 密码，固定为2个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_changepin_4428(HANDLE icdev, unsigned char *passwd);

  /**
   * @brief  修改4428卡密码。
   * @par    说明：
   * ::dc_changepin_4428 的HEX形式接口，参数 @a passwd 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_changepin_4428_hex(HANDLE icdev, unsigned char *passwd);

  /**
   * @brief  读4428卡保护位。
   * @par    说明：
   * 读取4428卡的保护区中哪些位置已经被置保护。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 读取长度。
   * @param[out] data_buffer 返回的数据，数据中含有0x00字节的位置表示已经被置保护。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_readprotect_4428(HANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  读4428卡保护位。
   * @par    说明：
   * ::dc_readprotect_4428 的HEX形式接口，参数 @a data_buffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_readprotect_4428_hex(HANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  写4428卡保护位。
   * @par    说明：
   * 对4428卡的保护区中指定位置进行置保护。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 写入长度。
   * @param[in] data_buffer 传入数据，数据中和卡内原有数据相同的字节位置将被置保护。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_writeprotect_4428(HANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  写4428卡保护位。
   * @par    说明：
   * ::dc_writeprotect_4428 的HEX形式接口，参数 @a data_buffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_writeprotect_4428_hex(HANDLE icdev, short offset, short length, unsigned char *data_buffer);

  /**
   * @brief  检测4442卡。
   * @par    说明：
   * 检测当前卡座中是否存在4442卡。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  API_ATTRIBUTE short USER_API dc_Check_4442(HANDLE icdev);

  /**
   * @brief  检测4428卡。
   * @par    说明：
   * 检测当前卡座中是否存在4428卡。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  API_ATTRIBUTE short USER_API dc_Check_4428(HANDLE icdev);

  /**
   * @brief  检测24C01卡。
   * @par    说明：
   * 检测当前卡座中是否存在24C01卡。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  API_ATTRIBUTE short USER_API dc_Check_24C01(HANDLE icdev);

  /**
   * @brief  检测24C02卡。
   * @par    说明：
   * 检测当前卡座中是否存在24C02卡。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  API_ATTRIBUTE short USER_API dc_Check_24C02(HANDLE icdev);

  /**
   * @brief  检测24C04卡。
   * @par    说明：
   * 检测当前卡座中是否存在24C04卡。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  API_ATTRIBUTE short USER_API dc_Check_24C04(HANDLE icdev);

  /**
   * @brief  检测24C08卡。
   * @par    说明：
   * 检测当前卡座中是否存在24C08卡。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  API_ATTRIBUTE short USER_API dc_Check_24C08(HANDLE icdev);

  /**
   * @brief  检测24C16卡。
   * @par    说明：
   * 检测当前卡座中是否存在24C16卡。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  API_ATTRIBUTE short USER_API dc_Check_24C16(HANDLE icdev);

  /**
   * @brief  检测24C64卡。
   * @par    说明：
   * 检测当前卡座中是否存在24C64卡。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  API_ATTRIBUTE short USER_API dc_Check_24C64(HANDLE icdev);

  /**
   * @brief  检测CPU卡。
   * @par    说明：
   * 检测当前卡座中是否存在CPU卡。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在T=0的CPU卡，==1表示存在T=1的CPU卡。
   */
  API_ATTRIBUTE short USER_API dc_Check_CPU(HANDLE icdev);

  /**
   * @brief  检测卡。
   * @par    说明：
   * 检测当前卡座中存在的卡类型。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败或不存在。
   * @n 8 - 表示存在4442卡。
   * @n 9 - 表示存在4428卡。
   * @n 30 - 表示存在T=0的CPU卡。
   * @n 31 - 表示存在T=1的CPU卡。
   * @n 21 - 表示存在24C01卡。
   * @n 22 - 表示存在24C02卡。
   * @n 23 - 表示存在24C04卡。
   * @n 24 - 表示存在24C08卡。
   * @n 25 - 表示存在24C16卡。
   * @n 26 - 表示存在24C64卡。
   */
  API_ATTRIBUTE short USER_API dc_CheckCard(HANDLE icdev);

  /**
   * @brief  获取设备长版本。
   * @par    说明：
   * 获取设备内部固件代码的长版本，此长版本依赖设备内部固件代码的实现，有可能同设备版本一致。
   * @param[in] icdev 设备标识符。
   * @param[out] sver 返回的版本字符串，请至少分配128个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_getlongver(HANDLE icdev, unsigned char *sver);

  /**
   * @brief  寻卡请求、防卡冲突、选卡操作。
   * @par    说明：
   * 内部包含了 ::dc_request ::dc_anticoll ::dc_select 的功能。
   * @param[in] icdev 设备标识符。
   * @param[in] _Mode 模式，同 ::dc_request 的 @a _Mode 。
   * @param[out] Strsnr 返回的卡序列号，格式为数字字符串。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  API_ATTRIBUTE short USER_API dc_cardstr(HANDLE icdev, unsigned char _Mode, char *Strsnr);

  /**
   * @brief  寻Type A或Type B卡并激活。
   * @par    说明：
   * 对Type A或Type B卡进行寻卡和激活。
   * @param[in] icdev 设备标识符。
   * @param[out] rlen 返回激活信息的长度。
   * @param[out] rbuf 返回的激活信息，请至少分配128个字节。
   * @param[out] type 类型，'A'表示Type A卡，'B'表示Type B卡。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  API_ATTRIBUTE short USER_API dc_cardAB(HANDLE icdev, unsigned char *rlen, unsigned char *rbuf, unsigned char *type);

  /**
   * @brief  寻Type B卡并激活。
   * @par    说明：
   * 对Type B卡进行寻卡和激活。
   * @param[in] icdev 设备标识符。
   * @param[out] rbuf 返回的激活信息，请至少分配128个字节。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  API_ATTRIBUTE short USER_API dc_card_b(HANDLE icdev, unsigned char *rbuf);

  /**
   * @brief  寻Type B卡并激活。
   * @par    说明：
   * ::dc_card_b 的HEX形式接口，参数 @a rbuf 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_card_b_hex(HANDLE icdev, char *rbuf);

  /**
   * @brief  液晶显示。
   * @par    说明：
   * 显示指定字符串到液晶屏幕上。
   * @param[in] icdev 设备标识符。
   * @param[in] line 行号。
   * @param[in] offset 偏移。
   * @param[in] data 要显示的字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_dispinfo_T8(HANDLE icdev, unsigned char line, unsigned char offset, char *data);

  /**
   * @brief  液晶显示。
   * @par    说明：
   * 显示指定字符串到液晶屏幕上。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 屏幕偏移。
   * @param[in] data 要显示的字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_dispinfo_pro_T8(HANDLE icdev, unsigned char offset, char *data);

  /**
   * @brief  LCD清屏。
   * @par    说明：
   * 清除LCD屏幕的显示内容。
   * @param[in] icdev 设备标识符。
   * @param[in] line 需要清除的行号，编号从1开始。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_clearlcd_T8(HANDLE icdev, unsigned char line);

  /**
   * @brief  指示灯点亮/熄灭。
   * @par    说明：
   * 点亮/熄灭设备的指示灯。
   * @param[in] icdev 设备标识符。
   * @param[in] cled 指示灯编号，0表示全部指示灯，1表示第一个指示灯，2表示第二个指示灯，以此类推。
   * @param[in] cflag 0-点亮，1-熄灭。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_led_T8(HANDLE icdev, unsigned char cled, unsigned char cflag);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_dispmap_T8(HANDLE icdev, unsigned char *mapdata);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_displow_T8(HANDLE icdev, unsigned char rs, unsigned char cdata);

  /**
   * @brief  读/写射频寄存器。
   * @par    说明：
   * 从射频寄存器读取值或写入值到射频寄存器中。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 读写标志，0x00表示读，0x01表示写。
   * @param[in] _Adr 寄存器地址。
   * @param[in,out] _Data 寄存器值，固定为1个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_rw_rfreg(HANDLE icdev, unsigned char flag, unsigned char _Adr, unsigned char *_Data);

  /**
   * @brief  读/写射频寄存器。
   * @par    说明：
   * ::dc_rw_rfreg 的HEX形式接口，参数 @a _Data 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_rw_rfreg_hex(HANDLE icdev, unsigned char flag, unsigned char _Adr, unsigned char *_Data);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_mulrequest_b(HANDLE icdev, unsigned char _Mode, unsigned char AFI, unsigned char *cardnum, unsigned char *mulATQB);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_hltb(HANDLE icdev, unsigned char *PUPI);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_set_poweroff(HANDLE icdev, unsigned int MsTimes, unsigned char TimerClock, unsigned char TimerReload);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * 对感应区CPU卡进行指令交互操作，注意此接口不封装卡协议部分，带CRC。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @param[in] timeout 超时值，此值只在部分设备的底层使用，单位为250毫秒，一般调用建议值为7。
   * @param[in] CRCSTU CRC值。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_pro_commandsourceCRC(HANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer, unsigned char timeout, unsigned char CRCSTU);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * ::dc_pro_commandsourceCRC 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_pro_commandsourceCRChex(HANDLE icdev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer, unsigned char timeout, unsigned char CRCSTU);

  /**
   * @brief  Mifare Plus卡设置个人化数据（0级）。
   * @par    说明：
   * 设置Mifare Plus卡的个人化数据。
   * @param[in] icdev 设备标识符。
   * @param[in] BNr 要写入的个人化数据块号。
   * @param[in] dataperso 要写入的数据，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_MFPL0_writeperso(HANDLE icdev, unsigned int BNr, unsigned char *dataperso);

  /**
   * @brief  Mifare Plus卡设置个人化数据（0级）。
   * @par    说明：
   * ::dc_MFPL0_writeperso 的HEX形式接口，参数 @a dataperso 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_MFPL0_writeperso_hex(HANDLE icdev, unsigned int BNr, unsigned char *dataperso);

  /**
   * @brief  Mifare Plus卡提交个人化数据（0级）。
   * @par    说明：
   * 提交Mifare Plus卡的个人化数据，提交成功后卡片进入1级状态。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_MFPL0_commitperso(HANDLE icdev);

  /**
   * @brief  验证Mifare Plus卡状态密码（1级）。
   * @par    说明：
   * 验证Mifare Plus卡状态密码，用于在1级状态下实现严格的认证。
   * @param[in] icdev 设备标识符。
   * @param[in] authkey 认证密码，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_MFPL1_authl1key(HANDLE icdev, unsigned char *authkey);

  /**
   * @brief  验证Mifare Plus卡状态密码（1级）。
   * @par    说明：
   * ::dc_MFPL1_authl1key 的HEX形式接口，参数 @a authkey 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_MFPL1_authl1key_hex(HANDLE icdev, unsigned char *authkey);

  /**
   * @brief  升级Mifare Plus卡状态到2级（1级）。
   * @par    说明：
   * 状态切换函数，执行该操作后，1级状态的卡片转换到2级。
   * @param[in] icdev 设备标识符。
   * @param[in] authkey 升级密码，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_MFPL1_switchtol2(HANDLE icdev, unsigned char *authkey);

  /**
   * @brief  升级Mifare Plus卡状态到3级（1级）。
   * @par    说明：
   * 状态切换函数，执行该操作后，1级状态的卡片转换到3级。
   * @param[in] icdev 设备标识符。
   * @param[in] authkey 升级密码，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_MFPL1_switchtol3(HANDLE icdev, unsigned char *authkey);

  /**
   * @brief  升级Mifare Plus卡状态到3级（2级）。
   * @par    说明：
   * 状态切换函数，执行该操作后，2级状态的卡片转换到3级。
   * @param[in] icdev 设备标识符。
   * @param[in] authkey 升级密码，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_MFPL2_switchtol3(HANDLE icdev, unsigned char *authkey);

  /**
   * @brief  验证Mifare Plus卡状态密码（3级）。
   * @par    说明：
   * 执行3级状态卡片认证，根据密码块号的不同，验证不同的密码。
   * @param[in] icdev 设备标识符。
   * @param[in] keyBNr 密码块号。
   * @param[in] authkey 认证密码，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_MFPL3_authl3key(HANDLE icdev, unsigned int keyBNr, unsigned char *authkey);

  /**
   * @brief  验证Mifare Plus卡状态密码（3级）。
   * @par    说明：
   * ::dc_MFPL3_authl3key 的HEX形式接口，参数 @a authkey 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_MFPL3_authl3key_hex(HANDLE icdev, unsigned int keyBNr, unsigned char *authkey);

  /**
   * @brief  验证Mifare Plus卡状态密码（3级）。
   * @par    说明：
   * 执行3级状态卡片认证，根据密码扇区号的不同，验证不同的密码。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 模式，0x00表示验证A密码，内部用0x4000+sectorBNr*2计算密码块，0x04表示验证B密码，内部用0x4000+sectorBNr*2+1计算密码块。
   * @param[in] sectorBNr 密码扇区号。
   * @param[in] authkey 认证密码，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_MFPL3_authl3sectorkey(HANDLE icdev, unsigned char mode, unsigned int sectorBNr, unsigned char *authkey);

  /**
   * @brief  验证Mifare Plus卡状态密码（3级）。
   * @par    说明：
   * ::dc_MFPL3_authl3key 的HEX形式接口，参数 @a authkey 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_MFPL3_authl3sectorkey_hex(HANDLE icdev, unsigned char mode, unsigned int sectorBNr, unsigned char *authkey);

  /**
   * @brief  读Mifare Plus卡数据（3级）。
   * @par    说明：
   * 在3级状态下读取Mifare Plus卡数据，可以连续读多块，每块16字节。
   * @param[in] icdev 设备标识符。
   * @param[in] BNr 起始块地址。
   * @param[in] Numblock 块数量，一般不大于6块。
   * @param[out] readdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_MFPL3_readinplain(HANDLE icdev, unsigned int BNr, unsigned char Numblock, unsigned char *readdata);

  /**
   * @brief  读Mifare Plus卡数据（3级）。
   * @par    说明：
   * ::dc_MFPL3_readinplain 的HEX形式接口，参数 @a readdata 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_MFPL3_readinplain_hex(HANDLE icdev, unsigned int BNr, unsigned char Numblock, unsigned char *readdata);

  /**
   * @brief  加密模式读Mifare Plus卡数据（3级）。
   * @par    说明：
   * 在3级状态下用加密模式读取Mifare Plus卡数据，可以连续读多块，每块16字节。
   * @param[in] icdev 设备标识符。
   * @param[in] BNr 起始块地址。
   * @param[in] Numblock 块数量，一般不大于6块。
   * @param[out] readdata 返回的数据。
   * @param[in] flag 标志，0x00表示加密数据内部解密后再返回，0x01表示加密数据直接返回。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_MFPL3_readencrypted(HANDLE icdev, unsigned int BNr, unsigned char Numblock, unsigned char *readdata, unsigned char flag);

  /**
   * @brief  加密模式读Mifare Plus卡数据（3级）。
   * @par    说明：
   * ::dc_MFPL3_readencrypted 的HEX形式接口，参数 @a readdata 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_MFPL3_readencrypted_hex(HANDLE icdev, unsigned int BNr, unsigned char Numblock, unsigned char *readdata, unsigned char flag);

  /**
   * @brief  写Mifare Plus卡数据（3级）。
   * @par    说明：
   * 在3级状态下写入数据到Mifare Plus卡中，可以连续写多块，每块16字节。
   * @param[in] icdev 设备标识符。
   * @param[in] BNr 起始块地址。
   * @param[in] Numblock 块数量，一般不大于6块。
   * @param[in] writedata 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_MFPL3_writeinplain(HANDLE icdev, unsigned int BNr, unsigned char Numblock, unsigned char *writedata);

  /**
   * @brief  写Mifare Plus卡数据（3级）。
   * @par    说明：
   * ::dc_MFPL3_writeinplain 的HEX形式接口，参数 @a writedata 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_MFPL3_writeinplain_hex(HANDLE icdev, unsigned int BNr, unsigned char Numblock, unsigned char *writedata);

  /**
   * @brief  加密模式写Mifare Plus卡数据（3级）。
   * @par    说明：
   * 在3级状态下用加密模式写入数据到Mifare Plus卡中，可以连续写多块，每块16字节。注意：写密码时，必须使用此函数并且块数量只能为1。
   * @param[in] icdev 设备标识符。
   * @param[in] BNr 起始块地址。
   * @param[in] Numblock 块数量，一般不大于6块。
   * @param[in] writedata 传入数据。
   * @param[in] flag 标志，0x00表示传入数据需内部加密后再使用，0x01表示传入数据直接使用。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_MFPL3_writeencrypted(HANDLE icdev, unsigned int BNr, unsigned char Numblock, unsigned char *writedata, unsigned char flag);

  /**
   * @brief  加密模式写Mifare Plus卡数据（3级）。
   * @par    说明：
   * ::dc_MFPL3_writeencrypted 的HEX形式接口，参数 @a writedata 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_MFPL3_writeencrypted_hex(HANDLE icdev, unsigned int BNr, unsigned char Numblock, unsigned char *writedata, unsigned char flag);

  /**
   * @brief  验证Mifare Ultralight C卡密码。
   * @par    说明：
   * 使用传入的密码来验证Mifare Ultralight C卡密码。
   * @param[in] icdev 设备标识符。
   * @param[in] key 密码，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_auth_ulc(HANDLE icdev, unsigned char *key);

  /**
   * @brief  验证Mifare Ultralight C卡密码。
   * @par    说明：
   * ::dc_auth_ulc 的HEX形式接口，参数 @a key 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_auth_ulc_hex(HANDLE icdev, unsigned char *key);

  /**
   * @brief  修改Mifare Ultralight C卡密码。
   * @par    说明：
   * 修改Mifare Ultralight C卡的密码。
   * @param[in] icdev 设备标识符。
   * @param[in] newkey 密码，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_changekey_ulc(HANDLE icdev, unsigned char *newkey);

  /**
   * @brief  修改Mifare Ultralight C卡密码。
   * @par    说明：
   * ::dc_changekey_ulc 的HEX形式接口，参数 @a newkey 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_changekey_ulc_hex(HANDLE icdev, unsigned char *newkey);

  /**
   * @brief  获取接触式CPU卡参数。
   * @par    说明：
   * 此接口可获取接触式CPU卡操作行为的参数。
   * @param[in] icdev 设备标识符。
   * @param[in] cputype 卡座编号，同 ::dc_setcpu 的 @a _Byte 。
   * @param[out] cpupro 卡协议编号，0x00表示T0，0x01表示T1，默认为0x00。
   * @param[out] cpuetu 卡复位波特率编号，0x5C表示9600，0x14表示38400，0x0E表示115200。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_getcpupara(HANDLE icdev, unsigned char cputype, unsigned char *cpupro, unsigned char *cpuetu);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * 对当前卡座CPU卡进行指令交互操作，注意此接口不封装卡协议部分。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_cpuapdusourceEXT(HANDLE icdev, short slen, unsigned char *sendbuffer, short *rlen, unsigned char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * ::dc_cpuapdusourceEXT 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_cpuapdusourceEXT_hex(HANDLE icdev, short slen, char *sendbuffer, short *rlen, char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * 对当前卡座CPU卡进行指令交互操作，注意此接口已封装卡协议部分。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_cpuapduEXT(HANDLE icdev, short slen, unsigned char *sendbuffer, short *rlen, unsigned char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * ::dc_cpuapduEXT 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_cpuapduEXT_hex(HANDLE icdev, short slen, char *sendbuffer, short *rlen, char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * 对当前卡座CPU卡进行指令交互操作，注意此接口已封装卡协议部分。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_cpuapduInt(HANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * ::dc_cpuapduInt 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_cpuapduInt_hex(HANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * 对感应区CPU卡进行指令交互操作，注意此接口已封装卡协议部分。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @param[in] timeout 超时值，此值只在部分设备的底层使用，单位为250毫秒，一般调用建议值为7。
   * @param[in] FG 分割值，此值只在部分设备的底层使用，单位为字节，一般调用建议值为64。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_pro_commandlinkEXT(HANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer, unsigned char timeout, unsigned char FG);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * ::dc_pro_commandlinkEXT 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_pro_commandlinkEXT_hex(HANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer, unsigned char timeout, unsigned char FG);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * 对感应区CPU卡进行指令交互操作，注意此接口已封装卡协议部分。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @param[in] timeout 超时值，此值只在部分设备的底层使用，单位为250毫秒，一般调用建议值为7。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_pro_commandlinkInt(HANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * ::dc_pro_commandlinkInt 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_pro_commandlinkInt_hex(HANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer, unsigned char timeout);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * 对感应区CPU卡进行指令交互操作，注意此接口不封装卡协议部分，此接口必须配合 ::dc_RfCrcModeConfig 使用。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @param[in] timeout 超时值，此值只在部分设备的底层使用，单位为250毫秒，一般调用建议值为7。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_exchangeblock(HANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * ::dc_exchangeblock 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_exchangeblock_hex(HANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_write1024(HANDLE icdev, unsigned int offset, unsigned int length, unsigned char *writebuffer);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_write1024_hex(HANDLE icdev, unsigned int offset, unsigned int length, unsigned char *writebuffer);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_read1024(HANDLE icdev, unsigned int offset, unsigned int length, unsigned char *databuffer);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_read1024_hex(HANDLE icdev, unsigned int offset, unsigned int length, unsigned char *databuffer);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_switch_linux(HANDLE icdev);

  /**
   * @brief  接触式CPU卡复位。
   * @par    说明：
   * 对当前卡座CPU卡进行复位操作，此复位为热复位。
   * @param[in] icdev 设备标识符。
   * @param[out] rlen 返回复位信息的长度。
   * @param[out] databuffer 返回的复位信息，请至少分配128个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_cpuhotreset(HANDLE icdev, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  接触式CPU卡复位。
   * @par    说明：
   * ::dc_cpuhotreset 的HEX形式接口，参数 @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_cpuhotreset_hex(HANDLE icdev, unsigned char *rlen, char *databuffer);

  /**
   * @brief  检测多卡状态。
   * @par    说明：
   * 检测设备中存在什么类型的卡片，支持磁卡、非接、接触、身份证。
   * @param[in] icdev 设备标识符。
   * @param[out] flag 状态标志，固定为2个字节，可以解析为一个无符号整数（Big-Endian），值为下面列出的一种或多种之和。
   * @n 0x0001 - 表示已刷磁卡。
   * @n 0x0002 - 表示卡座存在接触式CPU卡。
   * @n 0x0004 - 表示卡座存在接触式未知卡。
   * @n 0x0008 - 表示感应区存在身份证。
   * @n 0x0010 - 表示感应区存在激活前的Type A CPU卡或Type B CPU卡。
   * @n 0x0020 - 表示感应区存在激活后的Type A CPU卡或Type B CPU卡。
   * @n 0x0040 - 表示感应区存在激活前的M1卡。
   * @n 0x0100 - 表示感应区存在激活前的多张Type A CPU卡。
   * @n 0x0200 - 表示感应区存在激活前的多张M1卡。
   * @n 0x0400 - 表示感应区同时存在激活前的Type A CPU卡和M1卡。
   * @n 0x0800 - 表示刷磁卡失败。
   * @n 0x1000 - 表示启动刷磁卡模式处于关闭状态。
   * @n 0x2000 - 表示启动刷磁卡模式处于开启状态。
   * @n 0x4000 - 表示感应区身份证激活前后状态一致。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  API_ATTRIBUTE short USER_API dc_card_exist(HANDLE icdev, unsigned char *flag);

  /**
   * @brief  检测接触式卡存在。
   * @par    说明：
   * 检测接触式卡片是否存在于当前卡座中。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示存在，==1表示不存在。
   */
  API_ATTRIBUTE short USER_API dc_card_status(HANDLE icdev);

  /**
   * @brief  身份证、Type A CPU卡、Type B CPU卡检测。
   * @par    说明：
   * 检测感应区是否存在身份证、Type A CPU卡、Type B CPU卡。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示无卡，==1表示存在身份证，==2表示存在Type A CPU卡，==3表示存在Type B CPU卡。
   */
  API_ATTRIBUTE short USER_API dc_typeab_card_status(HANDLE icdev);

  /**
   * @brief  寻卡请求、防卡冲突、选卡操作。
   * @par    说明：
   * 内部包含了 ::dc_request ::dc_anticoll ::dc_select ::dc_anticoll2 ::dc_select2 ::dc_anticoll3 ::dc_select3 的功能。
   * @param[in] icdev 设备标识符。
   * @param[in] _Mode 模式，同 ::dc_request 的 @a _Mode 。
   * @param[out] SnrLen 返回卡序列号的长度。
   * @param[out] _Snr 返回的卡序列号，请至少分配16个字节。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  API_ATTRIBUTE short USER_API dc_card_n(HANDLE icdev, unsigned char _Mode, unsigned int *SnrLen, unsigned char *_Snr);

  /**
   * @brief  寻卡请求、防卡冲突、选卡操作。
   * @par    说明：
   * ::dc_card_n 的HEX形式接口，参数 @a _Snr 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_card_n_hex(HANDLE icdev, unsigned char _Mode, unsigned int *SnrLen, unsigned char *_Snr);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_card_n_number(HANDLE icdev, unsigned char _Mode, char *_Snr);

  /**
   * @brief  获取单个按键值。
   * @par    说明：
   * 获取设备键盘的按键值，每次调用仅能获取单个按键值。
   * @param[in] icdev 设备标识符。
   * @param[in] timeout 设备超时值，单位为秒。
   * @param[out] value 返回的按键值，请至少分配8个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_pass_key_value(HANDLE icdev, unsigned char timeout, char *value);

  /**
   * @brief  播放语音。
   * @par    说明：
   * 播放设备内置语音。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 语音段号，从1开始编号。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_play_voice(HANDLE icdev, unsigned char mode);

  /**
   * @brief  播放语音。
   * @par    说明：
   * 播放设备内置语音。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 语音段号，从1开始编号。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_playvoice(HANDLE icdev, unsigned char mode);

  /**
   * @brief  装载语音。
   * @par    说明：
   * 装载语音数据到设备内。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 标志，0表示第一包，1表示第二包，以此类推，0xFFFF表示结束包。
   * @param[in] slen 传入数据的长度，必须小于或等于1024，当标志为结束包的时候为0。
   * @param[in] sdata 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_load_voice(HANDLE icdev, int flag, int slen, const unsigned char *sdata);

  /**
   * @brief  磁条卡参数配置。
   * @par    说明：
   * 配置操作磁条卡的参数，参数仅针对自动输出的模式有效。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 模式，0x00表示不用输出，0x01表示自动输出。
   * @param[in] track1 1轨道参数。
   * @n bit0 - 轨道数据输出设置，0表示不输出，1表示输出。
   * @n bit1 - 轨道数据头设置，0表示无数据头，1表示有数据头。
   * @n bit2 - 轨道数据尾设置，0表示无数据尾，1表示有数据尾。
   * @n bit3 - 轨道数据换行符设置，0表示无换行符，1表示有换行符。
   * @n bit4 - 轨道数据等号与其后数据设置，0表示不输出，1表示输出。
   * @n bit5~bit7 - 保留。
   * @param[in] track2 2轨道参数。
   * @n bit0 - 轨道数据输出设置，0表示不输出，1表示输出。
   * @n bit1 - 轨道数据头设置，0表示无数据头，1表示有数据头。
   * @n bit2 - 轨道数据尾设置，0表示无数据尾，1表示有数据尾。
   * @n bit3 - 轨道数据换行符设置，0表示无换行符，1表示有换行符。
   * @n bit4 - 轨道数据等号与其后数据设置，0表示不输出，1表示输出。
   * @n bit5~bit7 - 保留。
   * @param[in] track3 3轨道参数。
   * @n bit0 - 轨道数据输出设置，0表示不输出，1表示输出。
   * @n bit1 - 轨道数据头设置，0表示无数据头，1表示有数据头。
   * @n bit2 - 轨道数据尾设置，0表示无数据尾，1表示有数据尾。
   * @n bit3 - 轨道数据换行符设置，0表示无换行符，1表示有换行符。
   * @n bit4 - 轨道数据等号与其后数据设置，0表示不输出，1表示输出。
   * @n bit5~bit7 - 保留。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_setmag(HANDLE icdev, unsigned char mode, unsigned char track1, unsigned char track2, unsigned char track3);

  /**
   * @brief  设置打印机参数。
   * @par    说明：
   * 设置操作打印机的参数。
   * @param[in] icdev 设备标识符。
   * @param[in] FontSize 字体设置，0x00表示字符大小为8*16，0x01表示字符大小为12*24，0x02表示字符大小为16*32。
   * @param[in] Alignment 对齐设置，0x00表示左对齐，0x01表示居中，0x02表示右对齐。
   * @param[in] LeftMargin 左边距设置，字符大小为8*16时<48，字符大小为12*24时<32，字符大小为16*32时<24。
   * @param[in] RightMargin 右边距设置，字符大小为8*16时<48，字符大小为12*24时<32，字符大小为16*32时<24。
   * @param[in] RowPitch 行间隔设置，单位为点。
   * @param[in] PrintOutRate 打印速度设置（0x00~0x03），0x00最快，0x03最慢。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_setprint(HANDLE icdev, unsigned char FontSize, unsigned char Alignment, unsigned char LeftMargin, unsigned char RightMargin, unsigned char RowPitch, unsigned char PrintOutRate);

  /**
   * @brief  查询打印机参数。
   * @par    说明：
   * 查询操作打印机的参数。
   * @param[in] icdev 设备标识符。
   * @param[out] FontSize 字体设置，0x00表示字符大小为8*16，0x01表示字符大小为12*24，0x02表示字符大小为16*32。
   * @param[out] Alignment 对齐设置，0x00表示左对齐，0x01表示居中，0x02表示右对齐。
   * @param[out] LeftMargin 左边距设置，字符大小为8*16时<48，字符大小为12*24时<32，字符大小为16*32时<24。
   * @param[out] RightMargin 右边距设置，字符大小为8*16时<48，字符大小为12*24时<32，字符大小为16*32时<24。
   * @param[out] RowPitch 行间隔设置，单位为点。
   * @param[out] PrintOutRate 打印速度设置（0x00~0x03），0x00最快，0x03最慢。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_checkprint(HANDLE icdev, unsigned char *FontSize, unsigned char *Alignment, unsigned char *LeftMargin, unsigned char *RightMargin, unsigned char *RowPitch, unsigned char *PrintOutRate);

  /**
   * @brief  打印字符。
   * @par    说明：
   * 打印文本字符信息。
   * @param[in] icdev 设备标识符。
   * @param[in] length 字符数据的长度，必须小于或等于480。
   * @param[in] character 字符数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_printcharacter(HANDLE icdev, unsigned short length, unsigned char *character);

  /**
   * @brief  打印一维码。
   * @par    说明：
   * 打印一维码信息。
   * @param[in] icdev 设备标识符。
   * @param[in] hight 一维码的高度。
   * @param[in] displayflag 显示类型，0x00表示图形在下文本在上，0x01表示图形在上文本在下，0x02表示只显示图形不显示文本。
   * @param[in] FontSize 字体设置，0x00表示字符大小为8*16，0x01表示字符大小为12*24，0x02表示字符大小为16*32。
   * @param[in] length 文本数据的长度，必须小于或等于15。
   * @param[in] Onedimensional 文本数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_printOnedimensional(HANDLE icdev, unsigned char hight, unsigned char displayflag, unsigned char FontSize, unsigned short length, unsigned char *Onedimensional);

  /**
   * @brief  打印图片。
   * @par    说明：
   * 打印图片信息。
   * @param[in] icdev 设备标识符。
   * @param[in] length 长度，单位为字节，必须小于或等于48，不能为0。
   * @param[in] height 高度，单位为点，必须小于100，不能为0。
   * @param[in] Alignment 对齐设置，0x00表示左对齐，0x01表示居中，0x02表示右对齐。
   * @param[in] LeftMargin 左边距设置，必须小于或等于48。
   * @param[in] RightMargin 右边距设置，必须小于或等于48。
   * @param[in] size 图片数据的大小。
   * @param[in] Picture 图片数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_printPicture(HANDLE icdev, unsigned char length, unsigned char height, unsigned char Alignment, unsigned char LeftMargin, unsigned char RightMargin, unsigned short size, unsigned char *Picture);

  /**
   * @brief  进纸。
   * @par    说明：
   * 进纸到设备内用于打印。
   * @param[in] icdev 设备标识符。
   * @param[in] RowPitch 进纸行数，单位为点。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_printenter(HANDLE icdev, unsigned char RowPitch);

  /**
   * @brief  获取打印机状态。
   * @par    说明：
   * 获取打印机的状态。
   * @param[in] icdev 设备标识符。
   * @param[out] paperstatus 纸状态，0x00表示不缺纸，0x01表示缺纸。
   * @param[out] tempturestatus 温度状态，0x00表示温度正常，0x01表示温度过高。
   * @param[out] zkstatus 字库状态。
   * @n 0x00 - 三种字库都存在。
   * @n 0x01 - 三种字库都不存在。
   * @n 0x02 - 存在8*16字库。
   * @n 0x03 - 存在12*24字库。
   * @n 0x04 - 存在16*32字库。
   * @n 0x05 - 存在8*16字库和12*24字库。
   * @n 0x06 - 存在8*16字库和16*32字库。
   * @n 0x07 - 存在12*24字库和16*32字库。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_printstatus(HANDLE icdev, unsigned char *paperstatus, unsigned char *tempturestatus, unsigned char *zkstatus);

  /**
   * @brief  设置2.4G模块波特率。
   * @par    说明：
   * 设置与2.4G模块之间通讯的波特率。
   * @param[in] icdev 设备标识符。
   * @param[in] baund 波特率。
   * @n 0x00 - 2400。
   * @n 0x01 - 4800。
   * @n 0x02 - 9600。
   * @n 0x03 - 14400。
   * @n 0x04 - 38400。
   * @n 0x05 - 57600。
   * @n 0x06 - 115200。
   * @n 0x07 - 256000。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_set2dot4Gbaund(HANDLE icdev, unsigned char baund);

  /**
   * @brief  查询2.4G模块波特率。
   * @par    说明：
   * 查询与2.4G模块之间通讯的波特率。
   * @param[in] icdev 设备标识符。
   * @param[out] baund 波特率。
   * @n 0x00 - 2400。
   * @n 0x01 - 4800。
   * @n 0x02 - 9600。
   * @n 0x03 - 14400。
   * @n 0x04 - 38400。
   * @n 0x05 - 57600。
   * @n 0x06 - 115200。
   * @n 0x07 - 256000。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_check2dot4Gbaund(HANDLE icdev, unsigned char *baund);

  /**
   * @brief  2.4G复位。
   * @par    说明：
   * 用2.4G进行复位操作，
   * @param[in] icdev 设备标识符。
   * @param[out] rlen 返回复位信息的长度。
   * @param[out] databuffer 返回的复位信息，请至少分配128个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_2dot4_reset(HANDLE icdev, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  2.4G复位。
   * @par    说明：
   * ::dc_2dot4_reset 的HEX形式接口，参数 @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_2dot4_resethex(HANDLE icdev, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  2.4G指令交互。
   * @par    说明：
   * 用2.4G进行指令交互操作。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] senddata 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] recvbuffer 返回的数据。
   * @param[in] timeout 设备超时值，单位为秒。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_2dot4_command(HANDLE icdev, unsigned int slen, unsigned char *senddata, unsigned int *rlen, unsigned char *recvbuffer, unsigned char timeout);

  /**
   * @brief  2.4G指令交互。
   * @par    说明：
   * ::dc_2dot4_command 的HEX形式接口，参数 @a senddata @a recvbuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_2dot4_command_hex(HANDLE icdev, unsigned int slen, unsigned char *senddata, unsigned int *rlen, unsigned char *recvbuffer, unsigned char timeout);

  /**
   * @brief  检测102卡。
   * @par    说明：
   * 检测是否存在102卡。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  API_ATTRIBUTE short USER_API dc_check_102(HANDLE icdev);

  /**
   * @brief  102卡下电。
   * @par    说明：
   * 对102卡进行下电操作。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_down_102(HANDLE icdev);

  /**
   * @brief  读102卡。
   * @par    说明：
   * 读取102卡的数据。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 读取长度。
   * @param[out] readdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_read_102(HANDLE icdev, unsigned char offset, unsigned char length, unsigned char *readdata);

  /**
   * @brief  读102卡。
   * @par    说明：
   * ::dc_read_102 的HEX形式接口，参数 @a readdata 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_read_102_hex(HANDLE icdev, unsigned char offset, unsigned char length, unsigned char *readdata);

  /**
   * @brief  写102卡。
   * @par    说明：
   * 写入数据到102卡中。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 写入长度。
   * @param[in] writedata 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_write_102(HANDLE icdev, unsigned char offset, unsigned char length, unsigned char *writedata);

  /**
   * @brief  写102卡。
   * @par    说明：
   * ::dc_write_102 的HEX形式接口，参数 @a writedata 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_write_102_hex(HANDLE icdev, unsigned char offset, unsigned char length, unsigned char *writedata);

  /**
   * @brief  验证102卡密码。
   * @par    说明：
   * 使用传入的密码来验证102卡密码。
   * @param[in] icdev 设备标识符。
   * @param[in] zone 密码区。
   * @n 0 - 表示总密码，密码长度为2个字节。
   * @n 11 - 表示一区擦除密码，密码长度为6个字节。
   * @n 12 - 表示二区擦除密码，密码长度为4个字节。
   * @param[in] password 密码。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_checkpass_102(HANDLE icdev, short zone, unsigned char *password);

  /**
   * @brief  验证102卡密码。
   * @par    说明：
   * ::dc_checkpass_102 的HEX形式接口，参数 @a password 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_checkpass_102_hex(HANDLE icdev, short zone, unsigned char *password);

  /**
   * @brief  修改102卡密码。
   * @par    说明：
   * 修改102卡的密码。
   * @param[in] icdev 设备标识符。
   * @param[in] zone 密码区。
   * @n 0 - 表示总密码，密码长度为2个字节。
   * @n 11 - 表示一区擦除密码，密码长度为6个字节。
   * @n 12 - 表示二区擦除密码，密码长度为4个字节。
   * @param[in] password 密码。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_changepass_102(HANDLE icdev, short zone, unsigned char *password);

  /**
   * @brief  修改102卡密码。
   * @par    说明：
   * ::dc_changepass_102 的HEX形式接口，参数 @a password 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_changepass_102_hex(HANDLE icdev, short zone, unsigned char *password);

  /**
   * @brief  读102卡密码计数。
   * @par    说明：
   * 读取102卡的密码计数，此计数值表示可以尝试验证密码的次数。
   * @param[in] icdev 设备标识符。
   * @param[in] zone 密码区。
   * @n 0 - 表示总密码。
   * @return <0表示失败，否则为密码计数值。
   */
  API_ATTRIBUTE short USER_API dc_readcount_102(HANDLE icdev, short zone);

  /**
   * @brief  102卡熔丝。
   * @par    说明：
   * 对102卡进行熔丝操作。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_fuse_102(HANDLE icdev);

  /**
   * @brief  检测1604卡。
   * @par    说明：
   * 检测是否存在1604卡。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  API_ATTRIBUTE short USER_API dc_check_1604(HANDLE icdev);

  /**
   * @brief  1604卡下电。
   * @par    说明：
   * 对1604卡进行下电操作。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_down_1604(HANDLE icdev);

  /**
   * @brief  读1604卡。
   * @par    说明：
   * 读取1604卡的数据。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 读取长度。
   * @param[out] readdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_read_1604(HANDLE icdev, unsigned int offset, unsigned int length, unsigned char *readdata);

  /**
   * @brief  读1604卡。
   * @par    说明：
   * ::dc_read_1604 的HEX形式接口，参数 @a readdata 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_read_1604_hex(HANDLE icdev, unsigned int offset, unsigned int length, unsigned char *readdata);

  /**
   * @brief  写1604卡。
   * @par    说明：
   * 写入数据到1604卡中。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 写入长度。
   * @param[in] writedata 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_write_1604(HANDLE icdev, unsigned int offset, unsigned int length, unsigned char *writedata);

  /**
   * @brief  写1604卡。
   * @par    说明：
   * ::dc_write_1604 的HEX形式接口，参数 @a writedata 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_write_1604_hex(HANDLE icdev, unsigned int offset, unsigned int length, unsigned char *writedata);

  /**
   * @brief  验证1604卡密码。
   * @par    说明：
   * 使用传入的密码来验证1604卡密码。
   * @param[in] icdev 设备标识符。
   * @param[in] zone 密码区。
   * @n 0 - 表示总密码。
   * @n 1 - 表示一区密码。
   * @n 2 - 表示二区密码。
   * @n 3 - 表示三区密码。
   * @n 4 - 表示四区密码。
   * @n 11 - 表示一区擦除密码。
   * @n 12 - 表示二区擦除密码。
   * @n 13 - 表示三区擦除密码。
   * @n 14 - 表示四区擦除密码。
   * @param[in] password 密码，固定为2个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_checkpass_1604(HANDLE icdev, short zone, unsigned char *password);

  /**
   * @brief  验证1604卡密码。
   * @par    说明：
   * ::dc_checkpass_1604 的HEX形式接口，参数 @a password 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_checkpass_1604_hex(HANDLE icdev, short zone, unsigned char *password);

  /**
   * @brief  修改1604卡密码。
   * @par    说明：
   * 修改1604卡的密码。
   * @param[in] icdev 设备标识符。
   * @param[in] zone 密码区。
   * @n 0 - 表示总密码。
   * @n 1 - 表示一区密码。
   * @n 2 - 表示二区密码。
   * @n 3 - 表示三区密码。
   * @n 4 - 表示四区密码。
   * @n 11 - 表示一区擦除密码。
   * @n 12 - 表示二区擦除密码。
   * @n 13 - 表示三区擦除密码。
   * @n 14 - 表示四区擦除密码。
   * @param[in] password 密码，固定为2个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_changepass_1604(HANDLE icdev, short zone, unsigned char *password);

  /**
   * @brief  修改1604卡密码。
   * @par    说明：
   * ::dc_changepass_1604 的HEX形式接口，参数 @a password 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_changepass_1604_hex(HANDLE icdev, short zone, unsigned char *password);

  /**
   * @brief  读1604卡密码计数。
   * @par    说明：
   * 读取1604卡的密码计数，此计数值表示可以尝试验证密码的次数。
   * @param[in] icdev 设备标识符。
   * @param[in] zone 密码区。
   * @n 0 - 表示总密码。
   * @n 1 - 表示一区密码。
   * @n 2 - 表示二区密码。
   * @n 3 - 表示三区密码。
   * @n 4 - 表示四区密码。
   * @n 11 - 表示一区擦除密码。
   * @n 12 - 表示二区擦除密码。
   * @n 13 - 表示三区擦除密码。
   * @n 14 - 表示四区擦除密码。
   * @return <0表示失败，否则为密码计数值。
   */
  API_ATTRIBUTE short USER_API dc_readcount_1604(HANDLE icdev, short zone);

  /**
   * @brief  1604卡熔丝。
   * @par    说明：
   * 对1604卡进行熔丝操作。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_fuse_1604(HANDLE icdev);

  /**
   * @brief  检测45DB系列卡。
   * @par    说明：
   * 检测是否存在45DB系列卡。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  API_ATTRIBUTE short USER_API dc_check_45db(HANDLE icdev);

  /**
   * @brief  45DB系列卡下电。
   * @par    说明：
   * 对45DB系列卡进行下电操作。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_down_45db(HANDLE icdev);

  /**
   * @brief  读45DB系列卡。
   * @par    说明：
   * 读取45DB系列卡的数据。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 读取长度。
   * @param[out] readdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_read_45db(HANDLE icdev, unsigned int offset, unsigned int length, unsigned char *readdata);

  /**
   * @brief  读45DB系列卡。
   * @par    说明：
   * ::dc_read_45db 的HEX形式接口，参数 @a readdata 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_read_45db_hex(HANDLE icdev, unsigned int offset, unsigned int length, unsigned char *readdata);

  /**
   * @brief  写45DB系列卡。
   * @par    说明：
   * 写入数据到45DB系列卡中。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移。
   * @param[in] length 写入长度。
   * @param[in] writedata 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_write_45db(HANDLE icdev, unsigned int offset, unsigned int length, unsigned char *writedata);

  /**
   * @brief  写45DB系列卡。
   * @par    说明：
   * ::dc_write_45db 的HEX形式接口，参数 @a writedata 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_write_45db_hex(HANDLE icdev, unsigned int offset, unsigned int length, unsigned char *writedata);

  /**
   * @brief  4428卡下电。
   * @par    说明：
   * 对4428卡进行下电操作。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_down_4428(HANDLE icdev);

  /**
   * @brief  4442卡下电。
   * @par    说明：
   * 对4442卡进行下电操作。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_down_4442(HANDLE icdev);

  /**
   * @brief  24C系列卡下电。
   * @par    说明：
   * 对24C系列卡进行下电操作。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_down_24c(HANDLE icdev);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_flash_card(HANDLE icdev);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_read_walletbalance(HANDLE icdev, unsigned char *walletvalue);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_consume_wallet(HANDLE icdev, unsigned char *walletvalue, unsigned char *secret);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_transfer_wallet(HANDLE icdev, unsigned char *walletvalue);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_authentication_key(HANDLE icdev, unsigned char *data, unsigned char *desdata);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_set_terminalnumber(HANDLE icdev, unsigned char length, unsigned char *serieldata);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_get_terminalnumber(HANDLE icdev, unsigned char length, unsigned char *serieldata);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_set_trademerchantcode(HANDLE icdev, unsigned char length, unsigned char *merchantcode);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_get_trademerchantcode(HANDLE icdev, unsigned char length, unsigned char *merchantcode);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_set_transactiontype(HANDLE icdev, unsigned char typecode);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_get_transactiontype(HANDLE icdev, unsigned char *typecode);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_set_tradesecretkey(HANDLE icdev, unsigned char *secretkey);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_get_tradesecretkey(HANDLE icdev, unsigned char *secretkey);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_set_hardwareserialnumber(HANDLE icdev, unsigned char length, unsigned char *hardwareserial);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_get_hardwareserialnumber(HANDLE icdev, unsigned char length, unsigned char *hardwareserial);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_reset_factory(HANDLE icdev);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_get_signmessageparameters(HANDLE icdev, unsigned char length, unsigned char *parameters);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_get_record(HANDLE icdev, unsigned char tablenumber, unsigned char recordnumber, unsigned char *recordquantity, unsigned char *recorddata);

  /**
   * @brief  启动读磁条卡。
   * @par    说明：
   * 使设备进入监测刷磁条的状态，接口本身会直接返回，而不会等待刷完磁条卡。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_startreadmag(HANDLE icdev);

  /**
   * @brief  停止读磁条卡。
   * @par    说明：
   * 在设备进入监测刷磁条的状态后，只有刷完磁条卡或调用此接口才能使得设备退出这种状态。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_stopreadmag(HANDLE icdev);

  /**
   * @brief  获取磁条卡数据。
   * @par    说明：
   * 获取设备已经读取到的磁条卡数据。
   * @param[in] icdev 设备标识符。
   * @param[out] pTrack1Data 返回的1轨道数据，数据格式为'\0'结尾的字符串。
   * @param[out] pTrack1Len 返回1轨道数据的长度，不含'\0'。
   * @param[out] pTrack2Data 返回的2轨道数据，数据格式为'\0'结尾的字符串。
   * @param[out] pTrack2Len 返回2轨道数据的长度，不含'\0'。
   * @param[out] pTrack3Data 返回的3轨道数据，数据格式为'\0'结尾的字符串。
   * @param[out] pTrack3Len 返回3轨道数据的长度，不含'\0'。
   * @return <0表示失败，==0表示成功，==1表示等待刷卡，==2表示处于停止刷卡状态。
   */
  API_ATTRIBUTE short USER_API dc_readmag(HANDLE icdev, unsigned char *pTrack1Data, unsigned int *pTrack1Len, unsigned char *pTrack2Data, unsigned int *pTrack2Len, unsigned char *pTrack3Data, unsigned int *pTrack3Len);

  /**
   * @brief  自动获取磁条卡数据。
   * @par    说明：
   * 内部包含了 ::dc_startreadmag ::dc_readmag ::dc_stopreadmag 的功能。
   * @param[in] icdev 设备标识符。
   * @param[in] ctime 接口超时值，单位为秒。
   * @param[out] pTrack1Data 返回的1轨道数据，数据格式为'\0'结尾的字符串。
   * @param[out] pTrack1Len 返回1轨道数据的长度，不含'\0'。
   * @param[out] pTrack2Data 返回的2轨道数据，数据格式为'\0'结尾的字符串。
   * @param[out] pTrack2Len 返回2轨道数据的长度，不含'\0'。
   * @param[out] pTrack3Data 返回的3轨道数据，数据格式为'\0'结尾的字符串。
   * @param[out] pTrack3Len 返回3轨道数据的长度，不含'\0'。
   * @return <0表示失败，==0表示成功，==1表示等待刷卡，==2表示处于停止刷卡状态。
   */
  API_ATTRIBUTE short USER_API dc_readmagcardallA(HANDLE icdev, unsigned char ctime, unsigned char *pTrack1Data, unsigned int *pTrack1Len, unsigned char *pTrack2Data, unsigned int *pTrack2Len, unsigned char *pTrack3Data, unsigned int *pTrack3Len);

  /**
   * @brief  设置密码键盘密钥。
   * @par    说明：
   * 对密码键盘内的密钥进行设置操作。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 标志。
   * @n 0x00 - 表示装载主密钥，明文装载。
   * @n 0x01 - 表示装载PIN密钥，密文装载（用主密钥加密）。
   * @n 0x02 - 表示装载MAC密钥，密文装载（用主密钥加密）。
   * @n 0x03 - 表示装载TDK密钥，密文装载（用主密钥加密）。
   * @n 0xF0 - 表示清空主密钥、PIN密钥、MAC密钥、TDK密钥。
   * @n 0xF1 - 表示清空PIN密钥。
   * @n 0xF2 - 表示清空MAC密钥。
   * @n 0xF3 - 表示清空TDK密钥。
   * @param[in] keylen 密钥数据的长度。
   * @param[in] keydata 密钥数据。
   * @param[in] mode 模式，0x00表示单DES密钥，0x01表示三DES密钥，0x02表示SM4密钥，0x03表示24字节三DES密钥。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_InstallKbKey(HANDLE icdev, unsigned char flag, unsigned char keylen, unsigned char *keydata, unsigned char mode);

  /**
   * @brief  设置当前密钥组。
   * @par    说明：
   * 设置密码键盘当前使用的密钥组。
   * @param[in] icdev 设备标识符。
   * @param[in] keyID 密钥组号，从0x00开始编号。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SetKbKeyID(HANDLE icdev, unsigned char keyID);

  /**
   * @brief  获取加密按键数据。
   * @par    说明：
   * 从密码键盘获取ANSI X9.8 PIN BLOCK加密的按键数据。
   * @param[in] icdev 设备标识符。
   * @param[in] ctime 设备超时值，单位为秒。
   * @param[in] panlen 卡号的长度。
   * @param[in] pandata 卡号。
   * @param[out] rlen 返回加密数据的长度。
   * @param[out] cpass 返回的加密数据。
   * @return <0表示失败，==0表示成功，==0xA1表示取消输入，==0xA2表示输入超时。
   */
  API_ATTRIBUTE short USER_API dc_GetEnPass(HANDLE icdev, unsigned char ctime, unsigned char panlen, unsigned char *pandata, unsigned char *rlen, unsigned char *cpass);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_get55data(HANDLE icdev, unsigned int slen, unsigned char *sdata, unsigned char *PacketType, unsigned int *rlen, unsigned char *rdata);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_CardIssuingScript(HANDLE icdev, unsigned int slen, unsigned char *sdata, unsigned char *TransactionIsInterrupted, unsigned char *TradingResult);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_SetEMVPara(HANDLE icdev, unsigned int slen, unsigned char *sdata);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_CalculateMACCheck(HANDLE icdev, unsigned char keyindex, unsigned char macmode, unsigned int datalen, unsigned char *srcdata, unsigned char *macdata);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_CalculateDes(HANDLE icdev, unsigned char keyindex, unsigned char enmode, unsigned short datalen, unsigned char *srcdata, unsigned char *desdata);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_CalculateSm4(HANDLE icdev, unsigned char keyindex, unsigned char enmode, unsigned short datalen, unsigned char *srcdata, unsigned char *sm4data);

  /**
   * @brief  用工作密钥加解密。
   * @par    说明：
   * 使用密码键盘的工作密钥进行数据加解密操作，仅支持单DES、三DES和24字节三DES算法，算法由设置密钥时确定，请参考 ::dc_InstallKbKey 。
   * @param[in] icdev 设备标识符。
   * @param[in] keytype 密钥类型，0x01表示PIN密钥，0x02表示MAC密钥，0x03表示TDK密钥。
   * @param[in] enmode 运算模式，0x00表示加密，0x01表示解密。
   * @param[in] sourcedata 传入数据，固定为8个字节。
   * @param[out] retdata 返回的数据，固定为8个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_CreatDesData(HANDLE icdev, unsigned char keytype, unsigned char enmode, unsigned char *sourcedata, unsigned char *retdata);

  /**
   * @brief  用工作密钥加解密。
   * @par    说明：
   * 使用密码键盘的工作密钥进行数据加解密操作，仅支持SM4算法，算法由设置密钥时确定，请参考 ::dc_InstallKbKey 。
   * @param[in] icdev 设备标识符。
   * @param[in] keytype 密钥类型，0x01表示PIN密钥，0x02表示MAC密钥，0x03表示TDK密钥。
   * @param[in] enmode 运算模式，0x00表示加密，0x01表示解密。
   * @param[in] sourcedata 传入数据，固定为16个字节。
   * @param[out] retdata 返回的数据，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_CreatSm4Data(HANDLE icdev, unsigned char keytype, unsigned char enmode, unsigned char *sourcedata, unsigned char *retdata);

  /**
   * @brief  用工作密钥加解密。
   * @par    说明：
   * 使用密码键盘的工作密钥进行数据加解密操作，仅支持单DES ECB和三DES ECB算法，内部会调用 ::dc_CreatDesData 。
   * @param[in] icdev 设备标识符。
   * @param[in] keytype 密钥类型，0x01表示PIN密钥，0x02表示MAC密钥，0x03表示TDK密钥。
   * @param[in] enmode 运算模式，0x00表示加密，0x01表示解密。
   * @param[in] slen 传入数据的长度。
   * @param[in] sourcedata 传入数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] retdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_CreatDesDataECB(HANDLE icdev, unsigned char keytype, unsigned char enmode, int slen, unsigned char *sourcedata, int *rlen, unsigned char *retdata);

  /**
   * @brief  用工作密钥加解密。
   * @par    说明：
   * 使用密码键盘的工作密钥进行数据加解密操作，仅支持SM4 ECB算法，内部会调用 ::dc_CreatSm4Data 。
   * @param[in] icdev 设备标识符。
   * @param[in] keytype 密钥类型，0x01表示PIN密钥，0x02表示MAC密钥，0x03表示TDK密钥。
   * @param[in] enmode 运算模式，0x00表示加密，0x01表示解密。
   * @param[in] slen 传入数据的长度。
   * @param[in] sourcedata 传入数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] retdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_CreatSm4DataECB(HANDLE icdev, unsigned char keytype, unsigned char enmode, int slen, unsigned char *sourcedata, int *rlen, unsigned char *retdata);

  /**
   * @brief  用工作密钥算MAC。
   * @par    说明：
   * 使用密码键盘的工作密钥进行数据算MAC操作，内部会调用 ::dc_CreatDesData ::dc_CreatSm4Data 。
   * @param[in] icdev 设备标识符。
   * @param[in] keytype 密钥类型，0x01表示PIN密钥，0x02表示MAC密钥，0x03表示TDK密钥。
   * @param[in] enmode 运算模式，0x00或0x01表示用DES POS ECB算MAC，0x02表示用SM4算MAC，0x03表示用DES X9.9算MAC，0x04表示用三DES X9.19算MAC，0x05表示用三DES POS ECB算MAC，0x06表示用24字节三DES X9.19算MAC，0x07表示用24字节三DES POS ECB算MAC。
   * @param[in] slen 传入数据的长度。
   * @param[in] sourcedata 传入数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] retdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_CreatMacDataECB(HANDLE icdev, unsigned char keytype, unsigned char enmode, int slen, unsigned char *sourcedata, int *rlen, unsigned char *retdata);

  /**
   * @brief  写SD存储。
   * @par    说明：
   * 写入数据到SD存储中，可以用作数据保存等。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移地址。
   * @param[in] length 写入长度。
   * @param[in] writebuffer 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_writeSD(HANDLE icdev, int offset, int length, unsigned char *writebuffer);

  /**
   * @brief  读SD存储。
   * @par    说明：
   * 读取SD存储中的数据。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移地址。
   * @param[in] length 读取长度。
   * @param[out] readbuffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_readSD(HANDLE icdev, int offset, int length, unsigned char *readbuffer);

  /**
   * @brief  设置USB模式。
   * @par    说明：
   * 设置USB通讯模式，必须重新对设备进行上电才能生效。
   * @param[in] icdev 设备标识符。
   * @param[in] ucmode 模式。
   * @n 0x00 - 表示中断发送和中断接收，此时VID=257B，PID=3010。
   * @n 0x01 - 表示控制发送和控制接收，此时VID=257B，PID=3011。
   * @n 0x02 - 表示控制发送和中断接收，此时VID=0471，PID=7002。
   * @n 0x03 - 表示控制发送和中断接收，此时VID=0471，PID=7003。
   * @n 0x04 - 表示控制发送和中断接收，此时VID=0471，PID=7003，支持磁条卡自动上传功能。
   * @n 0x05 - 虚拟串口。
   * @n 0x06 - 虚拟串口，带复合HID，此时HID的VID=0471，PID=7003。
   * @n 0x07 - 表示控制发送和中断接收，此时VID=0471，PID=7002，支持磁条卡自动上传功能。
   * @n 0x08 - 表示控制发送和中断接收，此时VID=257B，PID=3031。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SetUsbMode(HANDLE icdev, unsigned char ucmode);

  /**
   * @brief  外接密码键盘通讯。
   * @par    说明：
   * 与外接密码键盘进行通讯交互。
   * @param[in] icdev 设备标识符。
   * @param[in] ctime 设备超时值，单位为秒。
   * @param[in] senlen 发送数据的长度。
   * @param[in] source 发送数据。
   * @param[out] reclen 返回数据的长度。
   * @param[out] rdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_InstallKey(HANDLE icdev, unsigned char ctime, unsigned char senlen, unsigned char *source, unsigned char *reclen, unsigned char *rdata);

  /**
   * @brief  获取外接密码键盘的版本。
   * @par    说明：
   * 获取外接密码键盘的版本。
   * @param[in] icdev 设备标识符。
   * @param[out] version 返回的版本字符串，请至少分配128个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_IFD_GetVersion(HANDLE icdev, char *version);

  /**
   * @brief  获取外接密码键盘设备序列号。
   * @par    说明：
   * 获取外接密码键盘设备内部定制的序列号，设备默认序列号为空，只有预先定制的设备才会存在可用的序列号。
   * @param[in] icdev 设备标识符。
   * @param[out] serial_number 返回的序列号字符串，请至少分配33个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_IFD_ReadSerialNumber(HANDLE icdev, char *serial_number);

  /**
   * @brief  外接密码键盘播放语音。
   * @par    说明：
   * 播放外接密码键盘内置语音。
   * @param[in] icdev 设备标识符。
   * @param[in] _playmode 语音段号（0x01~0x08）。
   * @n 0x01 - "请输入密码"。
   * @n 0x02 - "请输入原密码"。
   * @n 0x03 - "请再输入一次"。
   * @n 0x04 - "请输入新密码"。
   * @n 0x05 - "请确认新密码"。
   * @n 0x06 - "密码修改成功"。
   * @n 0x07 - "请插卡"。
   * @n 0x08 - "请刷卡"。
   * @n 0x09 - "密码错误"。
   * @n 0x0A - "请扫码"。
   * @n 0x0B - "请扫电子社保卡"。
   * @n 0x0C - "支付成功"。
   * @n 0x0D - ""。
   * @n 0x0E - "支付失败"。
   * @n 0x0F - "扫码完成"。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_IFD_PlayVoice(HANDLE icdev, unsigned char _playmode);

  /**
   * @brief  外接密码键盘初始化。
   * @par    说明：
   * 对外接密码键盘进行初始化操作，使其复位为出厂状态，出厂状态下所有的主密钥内存空间都为字节0x88。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_IFD_Init(HANDLE icdev);

  /**
   * @brief  设置外接密码键盘主密钥。
   * @par    说明：
   * 对外接密码键盘内的主密钥进行设置操作。
   * @param[in] icdev 设备标识符。
   * @param[in] keyset 密钥组号，从0x00开始编号。
   * @param[in] oldkey 旧主密钥数据，明文，固定为16个字节。
   * @param[in] newkey 新主密钥数据，明文，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_IFD_SetMainKey(HANDLE icdev, unsigned char keyset, unsigned char *oldkey, unsigned char *newkey);

  /**
   * @brief  设置外接密码键盘工作密钥。
   * @par    说明：
   * 对外接密码键盘内的工作密钥进行设置操作。
   * @param[in] icdev 设备标识符。
   * @param[in] keysetmain 主密钥组号，从0x00开始编号。
   * @param[in] keysetwork 工作密钥组号，从0x00开始编号。
   * @param[in] enkeywork 工作密钥数据，密文（用主密钥加密），固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_IFD_SetWorkKey(HANDLE icdev, unsigned char keysetmain, unsigned char keysetwork, unsigned char *enkeywork);

  /**
   * @brief  激活外接密码键盘工作密钥。
   * @par    说明：
   * 激活外接密码键盘当前使用的工作密钥。
   * @param[in] icdev 设备标识符。
   * @param[in] keysetmain 主密钥组号，从0x00开始编号。
   * @param[in] keysetwork 工作密钥组号，从0x00开始编号。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_IFD_ActWorkKey(HANDLE icdev, unsigned char keysetmain, unsigned char keysetwork);

  /**
   * @brief  设置外接密码键盘最大输入长度。
   * @par    说明：
   * 设置外接密码键盘允许用户输入密码的最大长度。
   * @param[in] icdev 设备标识符。
   * @param[in] keylength 密码最大长度（1~16）。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_IFD_SetKeyLength(HANDLE icdev, unsigned char keylength);

  /**
   * @brief  获取外接密码键盘明文数据。
   * @par    说明：
   * 从外接密码键盘获取明文按键数据。
   * @param[in] icdev 设备标识符。
   * @param[out] szPasswd 返回的按键数据，数据格式为'\0'结尾的字符串。
   * @return <0表示失败，==0表示成功，==0xA1表示取消输入，==0xA2表示输入超时。
   */
  API_ATTRIBUTE short USER_API SD_IFD_GetPIN(HANDLE icdev, char *szPasswd);

  /**
   * @brief  获取外接密码键盘密文数据。
   * @par    说明：
   * 从外接密码键盘获取密文按键数据。
   * @param[in] icdev 设备标识符。
   * @param[out] szPasswd 返回的按键数据，数据为密文的HEX格式字符串。
   * @return <0表示失败，==0表示成功，==0xA1表示取消输入，==0xA2表示输入超时。
   */
  API_ATTRIBUTE short USER_API SD_IFD_GetEnPIN(HANDLE icdev, char *szPasswd);

  /**
   * @brief  获取外接密码键盘明文数据。
   * @par    说明：
   * 从外接密码键盘获取明文按键数据。
   * @param[in] icdev 设备标识符。
   * @param[out] szPasswd 返回的按键数据，数据格式为'\0'结尾的字符串。
   * @param[in] yyflag 标志。
   * @n 0x80 - 绿灯亮，液晶显示"欢迎使用"，无语音提示。
   * @n 0x81 - 绿灯亮，液晶显示"请再输入一次"，有语音提示。
   * @n 0x82 - 绿灯亮，液晶显示"请输入密码"，有语音提示。
   * @n 0x83 - 绿灯亮，液晶不操作，无语音提示。
   * @n 0x84 - 绿灯亮，液晶显示"请输入原密码"，有语音提示。
   * @n 0x85 - 绿灯亮，液晶显示"请输入新密码"，有语音提示。
   * @n 0x86 - 绿灯亮，液晶显示"请输入新密码"，无语音提示。
   * @param[in] timeout 设备超时值，单位为秒。
   * @return <0表示失败，==0表示成功，==0xA1表示取消输入，==0xA2表示输入超时。
   */
  API_ATTRIBUTE short USER_API SD_IFD_GetPINPro(HANDLE icdev, char *szPasswd, unsigned char yyflag, unsigned char timeout);

  /**
   * @brief  获取外接密码键盘密文数据。
   * @par    说明：
   * 从外接密码键盘获取密文按键数据。
   * @param[in] icdev 设备标识符。
   * @param[out] szPasswd 返回的按键数据，数据格式为'\0'结尾的字符串。
   * @param[in] yyflag 标志。
   * @n 0x30 - 密码键盘提示"请输入密码"，语音提示，输入密码显示'*'。
   * @n 0x31 - 密码键盘提示"请再输入一次"，语音提示，输入密码显示'*'。
   * @n 0x32 - 密码键盘提示"请输入密码"，无语音提示，输入密码显示'*'。
   * @n 0x33 - 密码键盘提示"请再输入一次"，无语音提示，输入密码显示'*'。
   * @n 0x34 - 密码键盘提示"请输入密码"，语音提示，输入密码明文显示。
   * @n 0x35 - 密码键盘提示"请再输入一次"，语音提示，输入密码明文显示。
   * @n 0x36 - 密码键盘提示"请输入密码"，无语音提示，输入密码明文显示。
   * @n 0x37 - 密码键盘提示"请再输入一次"，无语音提示，输入密码明文显示。
   * @param[in] modeflag 模式。
   * @n 0x30 - 按"确认"键或到达指定的PIN长度。
   * @n 0x31 - 按"确认"键。
   * @n 0x32 - 到达指定的PIN长度。
   * @n 0x33 - 到达指定的PIN长度并按"确认"键。
   * @param[in] timeout 设备超时值，单位为秒。
   * @return <0表示失败，==0表示成功，==0xA1表示取消输入，==0xA2表示输入超时。
   */
  API_ATTRIBUTE short USER_API SD_IFD_GetEnPINPro(HANDLE icdev, char *szPasswd, unsigned char yyflag, unsigned char modeflag, unsigned char timeout);

  /**
   * @brief  获取外接密码键盘密文数据。
   * @par    说明：
   * 从外接密码键盘获取ANSI X9.8 PIN BLOCK加密的按键数据。
   * @param[in] icdev 设备标识符。
   * @param[out] szPasswd 返回的按键数据，数据格式为'\0'结尾的字符串。
   * @param[in] yyflag 标志。
   * @n 0x30 - 密码键盘提示"请输入密码"，语音提示，输入密码显示'*'。
   * @n 0x31 - 密码键盘提示"请再输入一次"，语音提示，输入密码显示'*'。
   * @n 0x32 - 密码键盘提示"请输入密码"，无语音提示，输入密码显示'*'。
   * @n 0x33 - 密码键盘提示"请再输入一次"，无语音提示，输入密码显示'*'。
   * @n 0x34 - 密码键盘提示"请输入密码"，语音提示，输入密码明文显示。
   * @n 0x35 - 密码键盘提示"请再输入一次"，语音提示，输入密码明文显示。
   * @n 0x36 - 密码键盘提示"请输入密码"，无语音提示，输入密码明文显示。
   * @n 0x37 - 密码键盘提示"请再输入一次"，无语音提示，输入密码明文显示。
   * @param[in] modeflag 模式。
   * @n 0x30 - 按"确认"键或到达指定的PIN长度。
   * @n 0x31 - 按"确认"键。
   * @n 0x32 - 到达指定的PIN长度。
   * @n 0x33 - 到达指定的PIN长度并按"确认"键。
   * @param[in] cardno 截取后的卡号，固定为12个字节。
   * @param[in] timeout 设备超时值，单位为秒。
   * @return <0表示失败，==0表示成功，==0xA1表示取消输入，==0xA2表示输入超时。
   */
  API_ATTRIBUTE short USER_API SD_IFD_GetEnPINBlock(HANDLE icdev, char *szPasswd, unsigned char yyflag, unsigned char modeflag, unsigned char *cardno, unsigned char timeout);

  /**
   * @brief  获取外接密码键盘金额数据。
   * @par    说明：
   * 从外接密码键盘获取金额数据。
   * @param[in] icdev 设备标识符。
   * @param[out] amount 返回的金额数据，数据格式为'\0'结尾的字符串。
   * @param[in] timeout 设备超时值，单位为秒。
   * @return <0表示失败，==0表示成功，==0xA1表示取消输入，==0xA2表示输入超时。
   */
  API_ATTRIBUTE short USER_API SD_IFD_GetAmount(HANDLE icdev, char *amount, unsigned char timeout);

  /**
   * @brief  外接密码键盘启动按键。
   * @par    说明：
   * 外接密码键盘启动按键。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 输入模式。
   * @n 0x30 - 金额模式。
   * @n 0x31 - 功能模式。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_IFD_StartInput(HANDLE icdev, unsigned char mode);

  /**
   * @brief  获取外接密码键盘输入值。
   * @par    说明：
   * 获取外接密码键盘输入值。
   * @param[in] icdev 设备标识符。
   * @param[out] state 返回的状态，0x00表示已按【确认】键，0x01表示未按【确认】键。
   * @param[out] value 返回的输入值，格式为'\0'结尾的字符串。
   * @return <0表示失败，==0表示成功，==0xA1表示取消输入，==0xA2表示输入超时。
   */
  API_ATTRIBUTE short USER_API SD_IFD_GetInputValue(HANDLE icdev, unsigned char *state, char *value);

  /**
   * @brief  外接密码键盘退出按键。
   * @par    说明：
   * 外接密码键盘退出按键。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_IFD_ExitInput(HANDLE icdev);

  /**
   * @brief  设置外接密码键盘算法。
   * @par    说明：
   * 设置外接密码键盘使用的算法。
   * @param[in] icdev 设备标识符。
   * @param[in] desmode 模式，0x01表示单DES，0x02表示三DES，0x03表示SM4。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_IFD_SelecetDes(HANDLE icdev, unsigned char desmode);

  /**
   * @brief  外接密码键盘液晶显示。
   * @par    说明：
   * 显示指定字符串到外接密码键盘的液晶屏幕上。
   * @param[in] icdev 设备标识符。
   * @param[in] line 行号。
   * @param[in] row 偏移。
   * @param[in] leddata 要显示的字符串。
   * @param[in] timeout 设备超时值，单位为秒。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_IFD_LEDDisplay(HANDLE icdev, unsigned char line, unsigned char row, unsigned char *leddata, unsigned char timeout);

  /**
   * @brief  用外接密码键盘工作密钥加解密，建议使用 ::SD_IFD_Algorithm 代替。
   * @par    说明：
   * 使用外接密码键盘的工作密钥进行数据加解密操作。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 运算模式，0x00表示加密，0x01表示解密。
   * @param[in] srcdata 传入数据，HEX格式字符串。
   * @param[out] desdata 返回的数据，HEX格式字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_IFD_DES(HANDLE icdev, unsigned char mode, unsigned char *srcdata, unsigned char *desdata);

  /**
   * @brief  用外接密码键盘工作密钥加解密，建议使用 ::SD_IFD_Algorithm 代替。
   * @par    说明：
   * 使用外接密码键盘的工作密钥进行数据加解密操作。
   * @param[in] icdev 设备标识符。
   * @param[in] srclen 传入数据的长度。
   * @param[in] srcdata 传入数据。
   * @param[out] desdata 返回的数据，HEX格式字符串。
   * @param[in] flag 运算模式，0x00表示加密，0x01表示解密。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_IFD_DesCaculate(HANDLE icdev, unsigned char srclen, unsigned char *srcdata, unsigned char *desdata, unsigned char flag);

  /**
   * @brief  用外接密码键盘工作密钥加解密。
   * @par    说明：
   * 使用外接密码键盘的工作密钥进行数据加解密操作。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 运算模式，0x00表示加密，0x01表示解密。
   * @param[in] slen 传入数据的长度。
   * @param[in] sdata 传入数据。
   * @param[in] rlen 返回数据的长度。
   * @param[in] rdata 返回数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_IFD_Algorithm(HANDLE icdev, unsigned char flag, unsigned int slen, const unsigned char *sdata, unsigned int *rlen, unsigned char *rdata);

  /**
   * @brief  用外接密码键盘工作密钥算MAC。
   * @par    说明：
   * 使用外接密码键盘的工作密钥进行数据算MAC操作。
   * @param[in] icdev 设备标识符。
   * @param[in] srclen 传入数据的长度。
   * @param[in] srcdata 传入数据。
   * @param[out] macdata 返回的MAC字符串，请至少分配64个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_IFD_CreateMac(HANDLE icdev, unsigned char srclen, unsigned char *srcdata, unsigned char *macdata);

  /**
   * @brief  用外接密码键盘工作密钥算MAC。
   * @par    说明：
   * 使用外接密码键盘的工作密钥进行数据算MAC操作。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 运算模式，0x01表示用DES X9.9算MAC。
   * @param[in] data_len 传入数据的长度。
   * @param[in] data 传入数据。
   * @param[out] mac 返回的MAC字符串，请至少分配64个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_IFD_CreateMacPro(HANDLE icdev, unsigned char mode, unsigned int data_len, const unsigned char *data, char *mac);

  /**
   * @brief  外接密码键盘开始扫描二维码。
   * @par    说明：
   * 外接密码键盘开始扫描二维码。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 保留，固定为0x00。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_IFD_Scan2DBarcodeStart(HANDLE icdev, unsigned char mode);

  /**
   * @brief  外接密码键盘获取二维码数据。
   * @par    说明：
   * 外接密码键盘获取二维码数据。
   * @param[in] icdev 设备标识符。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_IFD_Scan2DBarcodeGetData(HANDLE icdev, int *rlen, unsigned char *rdata);

  /**
   * @brief  外接密码键盘退出扫描二维码。
   * @par    说明：
   * 外接密码键盘退出扫描二维码。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_IFD_Scan2DBarcodeExit(HANDLE icdev);

  /**
   * @brief  外接密码键盘获取屏幕属性。
   * @par    说明：
   * 外接密码键盘获取屏幕属性。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类别，0x00表示获取设备内部字模数据缓存字节大小，0x01表示获取屏幕X轴像素个数，0x02表示获取屏幕Y轴像素个数，0x03表示屏幕单个像素所占用位数。
   * @param[out] attribute_len 返回属性数据的长度。
   * @param[out] attribute 返回的属性数据。
   * @n type为0x00时，返回4字节缓存字节大小，高位在前。
   * @n type为0x01时，返回4字节屏幕X轴像素个数，高位在前。
   * @n type为0x02时，返回4字节屏幕Y轴像素个数，高位在前。
   * @n type为0x03时，返回1字节屏幕单个像素所占用位数。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_IFD_GetScreenAttribute(HANDLE icdev, unsigned char type, int *attribute_len, unsigned char *attribute);

  /**
   * @brief  外接密码键盘画屏。
   * @par    说明：
   * 外接密码键盘把像素图像画在屏幕上，只支持一次性画整个屏幕。
   * @param[in] icdev 设备标识符。
   * @param[in] pixel_len 像素数据的长度。
   * @param[in] pixel 像素数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API SD_IFD_DrawScreen(HANDLE icdev, int pixel_len, const unsigned char *pixel);

  /**
   * @brief  查找身份证。
   * @par    说明：
   * 以读取数据方式查找是否有身份证存在于感应区。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  API_ATTRIBUTE short USER_API dc_find_i_d(HANDLE icdev);

  /**
   * @brief  查找身份证。
   * @par    说明：
   * 以读取状态方式查找是否有身份证存在于感应区。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  API_ATTRIBUTE short USER_API dc_find_i_d_2(HANDLE icdev);

  /**
   * @brief  查找身份证。
   * @par    说明：
   * 以寻卡方式查找是否有身份证存在于感应区。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  API_ATTRIBUTE short USER_API dc_find_i_d_speed(HANDLE icdev);

  /**
   * @brief  读身份证原始数据。
   * @par    说明：
   * 读取身份证模块返回的原始数据，包含基础文字和相片内容。
   * @param[in] icdev 设备标识符。
   * @param[out] rdata 返回的数据，请至少分配4096个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_get_i_d_raw_buffer(HANDLE icdev, unsigned char *rdata);

  /**
   * @brief  读身份证。
   * @par    说明：
   * 读取身份证模块返回的原始数据，包含基础文字和相片内容，内部会做解析并且分配资源来保存，释放资源请使用 ::dc_end_i_d 。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，否则为身份证标识符。
   */
  API_ATTRIBUTE HANDLE USER_API dc_start_i_d(HANDLE icdev);

  /**
   * @brief  读身份证。
   * @par    说明：
   * 读取身份证模块返回的原始数据，包含基础文字、相片、指纹内容，内部会做解析并且分配资源来保存，释放资源请使用 ::dc_end_i_d 。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，否则为身份证标识符。
   */
  API_ATTRIBUTE HANDLE USER_API dc_start_i_d_2(HANDLE icdev);

  /**
   * @brief  获取姓名。
   * @par    说明：
   * 获取内部保存的姓名。
   * @param[in] idhandle 身份证标识符。
   * @return 返回的姓名字符串。
   */
  API_ATTRIBUTE char *USER_API dc_i_d_query_name(HANDLE idhandle);

  /**
   * @brief  获取姓名。
   * @par    说明：
   * 获取内部保存的姓名。
   * @param[in] idhandle 身份证标识符。
   * @param[out] name 姓名，请至少分配32个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_i_d_query_name_2(HANDLE idhandle, char *name);

  /**
   * @brief  获取性别。
   * @par    说明：
   * 获取内部保存的性别。
   * @param[in] idhandle 身份证标识符。
   * @return 返回的性别字符串。
   */
  API_ATTRIBUTE char *USER_API dc_i_d_query_sex(HANDLE idhandle);

  /**
   * @brief  获取性别。
   * @par    说明：
   * 获取内部保存的性别。
   * @param[in] idhandle 身份证标识符。
   * @param[out] sex 性别，请至少分配8个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_i_d_query_sex_2(HANDLE idhandle, char *sex);

  /**
   * @brief  获取民族。
   * @par    说明：
   * 获取内部保存的民族。
   * @param[in] idhandle 身份证标识符。
   * @return 返回的民族字符串。
   */
  API_ATTRIBUTE char *USER_API dc_i_d_query_nation(HANDLE idhandle);

  /**
   * @brief  获取民族。
   * @par    说明：
   * 获取内部保存的民族。
   * @param[in] idhandle 身份证标识符。
   * @param[out] nation 民族，请至少分配32个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_i_d_query_nation_2(HANDLE idhandle, char *nation);

  /**
   * @brief  获取生日。
   * @par    说明：
   * 获取内部保存的生日。
   * @param[in] idhandle 身份证标识符。
   * @return 返回的生日字符串。
   */
  API_ATTRIBUTE char *USER_API dc_i_d_query_birth(HANDLE idhandle);

  /**
   * @brief  获取生日。
   * @par    说明：
   * 获取内部保存的生日。
   * @param[in] idhandle 身份证标识符。
   * @param[out] birth 生日，请至少分配16个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_i_d_query_birth_2(HANDLE idhandle, char *birth);

  /**
   * @brief  获取住址。
   * @par    说明：
   * 获取内部保存的住址。
   * @param[in] idhandle 身份证标识符。
   * @return 返回的住址字符串。
   */
  API_ATTRIBUTE char *USER_API dc_i_d_query_address(HANDLE idhandle);

  /**
   * @brief  获取住址。
   * @par    说明：
   * 获取内部保存的住址。
   * @param[in] idhandle 身份证标识符。
   * @param[out] address 住址，请至少分配128个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_i_d_query_address_2(HANDLE idhandle, char *address);

  /**
   * @brief  获取公民身份号码。
   * @par    说明：
   * 获取内部保存的公民身份号码。
   * @param[in] idhandle 身份证标识符。
   * @return 返回的公民身份号码字符串。
   */
  API_ATTRIBUTE char *USER_API dc_i_d_query_id_number(HANDLE idhandle);

  /**
   * @brief  获取公民身份号码。
   * @par    说明：
   * 获取内部保存的公民身份号码。
   * @param[in] idhandle 身份证标识符。
   * @param[out] id_number 公民身份号码，请至少分配32个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_i_d_query_id_number_2(HANDLE idhandle, char *id_number);

  /**
   * @brief  获取签发机关。
   * @par    说明：
   * 获取内部保存的签发机关。
   * @param[in] idhandle 身份证标识符。
   * @return 返回的签发机关字符串。
   */
  API_ATTRIBUTE char *USER_API dc_i_d_query_department(HANDLE idhandle);

  /**
   * @brief  获取签发机关。
   * @par    说明：
   * 获取内部保存的签发机关。
   * @param[in] idhandle 身份证标识符。
   * @param[out] department 签发机关，请至少分配32个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_i_d_query_department_2(HANDLE idhandle, char *department);

  /**
   * @brief  获取有效期限。
   * @par    说明：
   * 获取内部保存的有效期限。
   * @param[in] idhandle 身份证标识符。
   * @return 返回的有效期限字符串。
   */
  API_ATTRIBUTE char *USER_API dc_i_d_query_expire_day(HANDLE idhandle);

  /**
   * @brief  获取有效期限。
   * @par    说明：
   * 获取内部保存的有效期限。
   * @param[in] idhandle 身份证标识符。
   * @param[out] expire_day 有效期限，请至少分配32个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_i_d_query_expire_day_2(HANDLE idhandle, char *expire_day);

  /**
   * @brief  获取相片原始数据。
   * @par    说明：
   * 获取内部保存的相片原始数据，此数据需要通过公安部相片解码库解码才能生成相片图像。
   * @param[in] idhandle 身份证标识符。
   * @return 返回的相片原始数据，数据实际长度通过调用 ::dc_i_d_query_photo_len 来获取。
   */
  API_ATTRIBUTE unsigned char *USER_API dc_i_d_query_photo(HANDLE idhandle);

  /**
   * @brief  获取相片原始数据。
   * @par    说明：
   * 获取内部保存的相片原始数据，此数据需要通过公安部相片解码库解码才能生成相片图像。
   * @param[in] idhandle 身份证标识符。
   * @param[out] photo 相片原始数据，请至少分配4096个字节，数据实际长度通过调用 ::dc_i_d_query_photo_len 来获取。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_i_d_query_photo_2(HANDLE idhandle, unsigned char *photo);

  /**
   * @brief  获取相片原始数据长度。
   * @par    说明：
   * 获取内部保存的相片原始数据的长度。
   * @param[in] idhandle 身份证标识符。
   * @return 返回的相片原始数据长度。
   */
  API_ATTRIBUTE unsigned int USER_API dc_i_d_query_photo_len(HANDLE idhandle);

  /**
   * @brief  生成相片图像文件。
   * @par    说明：
   * 使用内部保存的相片原始数据，通过调用公安部相片解码库解码生成相片图像文件。
   * @param[in] idhandle 身份证标识符。
   * @param[in] FileName 文件名，请确保有写入的权限。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_i_d_query_photo_file(HANDLE idhandle, const char *FileName);

  /**
   * @brief  生成相片图像数据。
   * @par    说明：
   * 使用内部保存的相片原始数据，通过调用公安部相片解码库解码生成相片图像数据。
   * @param[in] idhandle 身份证标识符。
   * @param[out] BmpBuffer 返回的相片图像数据，请至少分配65536个字节。
   * @param[in,out] BmpLength 传入 @a BmpBuffer 分配的字节数，返回相片图像数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_i_d_query_photo_bmp_buffer(HANDLE idhandle, unsigned char *BmpBuffer, unsigned int *BmpLength);

  /**
   * @brief  获取指纹原始数据。
   * @par    说明：
   * 获取内部保存的指纹原始数据。
   * @param[in] idhandle 身份证标识符。
   * @return 返回的指纹原始数据，数据实际长度通过调用 ::dc_i_d_query_finger_len 来获取。
   */
  API_ATTRIBUTE unsigned char *USER_API dc_i_d_query_finger(HANDLE idhandle);

  /**
   * @brief  获取指纹原始数据。
   * @par    说明：
   * 获取内部保存的指纹原始数据。
   * @param[in] idhandle 身份证标识符。
   * @param[out] finger 指纹原始数据，请至少分配4096个字节，数据实际长度通过调用 ::dc_i_d_query_finger_len 来获取。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_i_d_query_finger_2(HANDLE idhandle, unsigned char *finger);

  /**
   * @brief  获取指纹原始数据长度。
   * @par    说明：
   * 获取内部保存的指纹原始数据的长度。
   * @param[in] idhandle 身份证标识符。
   * @return 返回的指纹原始数据长度。
   */
  API_ATTRIBUTE unsigned int USER_API dc_i_d_query_finger_len(HANDLE idhandle);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_i_d_query_finger_file(HANDLE idhandle, const char *FileName);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_i_d_query_finger_bmp_buffer(HANDLE idhandle, unsigned char *BmpBuffer, unsigned int *BmpLength);

  /**
   * @brief  释放读身份证资源。
   * @par    说明：
   * 释放 ::dc_start_i_d ::dc_start_i_d_2 分配的资源。
   * @param[in] idhandle 身份证标识符。
   */
  API_ATTRIBUTE void USER_API dc_end_i_d(HANDLE idhandle);

  /**
   * @brief  获取身份证UID。
   * @par    说明：
   * 获取身份证的唯一标识，长度为8个字节。
   * @param[in] icdev 设备标识符。
   * @param[out] uid 返回的UID，固定为8个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_getuid_i_d(HANDLE icdev, unsigned char *uid);

  /**
   * @brief  LCD显示二维码。
   * @par    说明：
   * 在设备LCD显示屏上显示二维码。
   * @param[in] icdev 设备标识符。
   * @param[in] point_x X轴偏移点数（0~60）。
   * @param[in] point_y Y轴偏移点数（0~5）。
   * @param[in] info 文本字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_display_2dbarcode(HANDLE icdev, int point_x, int point_y, const char *info);

  /**
   * @brief  读身份证指纹。
   * @par    说明：
   * 读取身份证中的指纹数据。
   * @param[in] icdev 设备标识符。
   * @param[out] length 返回指纹数据的长度，不含'\0'。
   * @param[out] data 返回的指纹数据，HEX格式字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_getfingerdata(HANDLE icdev, int *length, unsigned char *data);

  /**
   * @brief  检测卡片位置。
   * @par    说明：
   * 检测卡片在设备中的位置。
   * @param[in] icdev 设备标识符。
   * @param[out] pos 位置。
   * @n 0x01 - 卡到开始位。
   * @n 0x02 - 卡到末位。
   * @n 0x03 - 卡已锁住。
   * @return <0表示失败，==0表示成功，==1表示无卡。
   */
  API_ATTRIBUTE short USER_API dc_card_status2(HANDLE icdev, unsigned char *pos);

  /**
   * @brief  开卡与锁卡。
   * @par    说明：
   * 开卡与锁卡。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 标志，0x00表示锁卡，0x01表示开卡。
   * @return <0表示失败，==0表示成功，==1表示无卡。
   */
  API_ATTRIBUTE short USER_API dc_card_make_and_lock(HANDLE icdev, unsigned char flag);

  /**
   * @brief  发送短信。
   * @par    说明：
   * 发送短信给指定号码。
   * @param[in] icdev 设备标识符。
   * @param[in] phone_no 号码。
   * @param[in] sms_content 短信内容。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_send_sms(HANDLE icdev, const char *phone_no, const char *sms_content);

  /**
   * @brief  指纹模块通讯。
   * @par    说明：
   * 与指纹模块进行通讯交互。
   * @param[in] icdev 设备标识符。
   * @param[in] ctime 设备超时值，单位为秒。
   * @param[in] request 发送数据。
   * @param[in] request_len 发送数据的长度。
   * @param[out] response 返回的数据。
   * @param[out] response_len 返回数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_fingerprint_io_control(HANDLE icdev, unsigned char ctime, const unsigned char *request, int request_len, unsigned char *response, int *response_len);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_keypad_io_control(HANDLE icdev, unsigned char ctime, const unsigned char *request, int request_len, unsigned char *response, int *response_len);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_keypad_ImportKey(HANDLE icdev, unsigned char *check_value, unsigned char check_mode, int key_index, int use_key_index, unsigned char enable, const unsigned char *key, int key_len);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_keypad_DeleteKey(HANDLE icdev, int key_index);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_keypad_SetButtonMark(HANDLE icdev, unsigned int mark_value, unsigned char use);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_keypad_Start(HANDLE icdev, unsigned char mode, int max_len, unsigned char auto_end, unsigned char enable_sound);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_keypad_End(HANDLE icdev);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_keypad_GetPressed(HANDLE icdev, int time_ms, unsigned char *value);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_keypad_GetPinBlock(HANDLE icdev, unsigned char mode, int key_index, const unsigned char *customer_data, int customer_data_len, unsigned char *out_data, int *out_data_len);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_keypad_GetRandomValue(HANDLE icdev, int length, unsigned char *value);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_keypad_GetEncryptDataOfCBC(HANDLE icdev, unsigned char mode, int key_index, const unsigned char *in_data, int in_data_len, unsigned char *out_data, int *out_data_len);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_keypad_GetDecryptDataOfCBC(HANDLE icdev, unsigned char mode, int key_index, const unsigned char *in_data, int in_data_len, unsigned char *out_data, int *out_data_len);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_keypad_GetEncryptDataOfECB(HANDLE icdev, unsigned char mode, int key_index, const unsigned char *in_data, int in_data_len, unsigned char *out_data, int *out_data_len);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_keypad_GetDecryptDataOfECB(HANDLE icdev, unsigned char mode, int key_index, const unsigned char *in_data, int in_data_len, unsigned char *out_data, int *out_data_len);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_keypad_GetMACDataOfCBC(HANDLE icdev, unsigned char mode, int key_index, const unsigned char *in_data, int in_data_len, unsigned char *out_data, int *out_data_len);

  /**
   * @brief  获取非接触卡类型。
   * @par    说明：
   * 获取感应区中存在的非接触卡类型，注意：此接口需要在卡激活后才能返回正确的卡类型，否则会返回0x00。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败。
   * @n 0x00 - 无卡。
   * @n 0x11 - Type A CPU Card。
   * @n 0x13 - Type A Mifare S50。
   * @n 0x14 - Type A Mifare S70。
   * @n 0x15 - Type A Mifare Ultralight。
   * @n 0x21 - Type B CPU Card。
   * @n 0x22 - Type B 存储卡。
   */
  API_ATTRIBUTE short USER_API dc_RfGetCardType(HANDLE icdev);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_RfWaveControl(HANDLE icdev, unsigned char mode);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_RfReadChipRegister(HANDLE icdev, unsigned char *register_data);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_RfSetChipRegister(HANDLE icdev, unsigned char register_type, unsigned char register_value);

  /**
   * @brief  获取身份证UID。
   * @par    说明：
   * 获取身份证的唯一标识，长度为8个字节。
   * @param[in] icdev 设备标识符。
   * @param[out] _Data 返回的UID，固定为8个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_get_idsnr(HANDLE icdev, unsigned char *_Data);

  /**
   * @brief  获取身份证UID。
   * @par    说明：
   * ::dc_get_idsnr 的HEX形式接口，参数 @a _Data 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_get_idsnr_hex(HANDLE icdev, char *_Data);

  /**
   * @brief  密码键盘模块通讯。
   * @par    说明：
   * 与密码键盘模块进行通讯交互。
   * @param[in] icdev 设备标识符。
   * @param[in] time_100ms 设备超时值，单位为100毫秒。
   * @param[in] slen 发送数据的长度。
   * @param[in] sdata 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_PinpadCommunication(HANDLE icdev, int time_100ms, int slen, const unsigned char *sdata, int *rlen, unsigned char *rdata);

  /**
   * @brief  透传通讯。
   * @par    说明：
   * 透传通讯。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类型，0x00表示P3键盘，0x01表示Android。
   * @param[in] time_100ms 设备超时值，单位为100毫秒。
   * @param[in] slen 发送数据的长度。
   * @param[in] sdata 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_PassthroughCommunication(HANDLE icdev, unsigned char type, int time_100ms, int slen, const unsigned char *sdata, int *rlen, unsigned char *rdata);

  /**
   * @brief  读Flash。
   * @par    说明：
   * 读取设备内部Flash中的数据。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移地址。
   * @param[in] length 读取长度。
   * @param[out] data_buffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ReadFlash(HANDLE icdev, int offset, int length, unsigned char *data_buffer);

  /**
   * @brief  写Flash。
   * @par    说明：
   * 写入数据到设备内部Flash中，可以用作数据保存等。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移地址。
   * @param[in] length 写入长度。
   * @param[in] data_buffer 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_WriteFlash(HANDLE icdev, int offset, int length, const unsigned char *data_buffer);

  /**
   * @brief  检测卡位置状态。
   * @par    说明：
   * 检测卡片当前的位置状态。
   * @param[in] icdev 设备标识符。
   * @param[out] pos 位置状态，可为多个以下值按位或在一起。
   * @n 0x00 - 无卡。
   * @n 0x01 - 无卡，卡在前门口，处夹卡位置。
   * @n 0x02 - 无卡，卡在前门口，处不夹卡位置。
   * @n 0x10 - 有卡，不可操作任何卡。
   * @n 0x11 - 有卡，可操作磁条。
   * @n 0x12 - 有卡，可操作接触。
   * @n 0x14 - 有卡，可操作非接触。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SelfServiceDeviceCardStatus(HANDLE icdev, unsigned char *pos);

  /**
   * @brief  进入卡片。
   * @par    说明：
   * 进入卡片，操作前设备内有卡则错误。
   * @param[in] icdev 设备标识符。
   * @param[in] time_s 设备超时值，单位为秒。
   * @param[in] mode 模式。
   * @n 0x00 - 前端进卡，不带磁条，不带IC。
   * @n 0x01 - 前端进卡，带磁条，不带IC。
   * @n 0x02 - 后端进卡，不带磁条。
   * @n 0x03 - 后端进卡，带磁条。
   * @n 0x04 - 前端进卡，不带磁条，带IC。
   * @n 0x05 - 前端进卡，带磁条，带IC。
   * @return <0表示失败。
   * @n 0 - 操作成功。
   * @n 1 - 设备内已有卡。
   * @n 2 - 接收超时。
   * @n 3 - 读磁卡错误。
   * @n 4 - 参数设置出错。
   * @n 5 - 异常卡正常弹出。
   * @n 6 - 异常卡卡在设备内。
   */
  API_ATTRIBUTE short USER_API dc_SelfServiceDeviceCardInject(HANDLE icdev, unsigned char time_s, unsigned char mode);

  /**
   * @brief  弹出卡片。
   * @par    说明：
   * 弹出卡片，操作前设备内无卡则错误。
   * @param[in] icdev 设备标识符。
   * @param[in] time_s 设备超时值，单位为秒。
   * @param[in] mode 模式。
   * @n 0x00 - 弹到前端并夹卡。
   * @n 0x01 - 弹到后端。
   * @n 0x02 - 弹到前端不夹卡。
   * @return <0表示失败。
   * @n 0 - 操作成功。
   * @n 1 - 设备内没有卡。
   * @n 2 - 接收超时。
   * @n 3 - 参数设置出错。
   * @n 4 - 卡片已到前门夹卡位置，可操作卡片。
   * @n 5 - 卡片已到前门不夹卡位置，不可操作卡片。
   */
  API_ATTRIBUTE short USER_API dc_SelfServiceDeviceCardEject(HANDLE icdev, unsigned char time_s, unsigned char mode);

  /**
   * @brief  移动卡片。
   * @par    说明：
   * 移动卡片，操作前设备内无卡则错误。
   * @param[in] icdev 设备标识符。
   * @param[in] time_s 设备超时值，单位为秒。
   * @param[in] mode 模式。
   * @n 0x00 - 移动到磁条卡操作位置。
   * @n 0x01 - 移动到接触卡操作位置。
   * @n 0x02 - 移动到非接触卡操作位置。
   * @return <0表示失败。
   * @n 0 - 操作成功。
   * @n 1 - 设备内没有卡。
   * @n 2 - 接收超时。
   * @n 3 - 参数设置出错。
   * @n 4 - 卡片已到前门不夹卡位置，不可操作卡片。
   */
  API_ATTRIBUTE short USER_API dc_SelfServiceDeviceCardMove(HANDLE icdev, unsigned char time_s, unsigned char mode);

  /**
   * @brief  获取传感器状态。
   * @par    说明：
   * 获取设备传感器的状态。
   * @param[in] icdev 设备标识符。
   * @param[out] value 状态值。
   * @n bit0 - 电闸门开关传感器，0表示打开，1表示关闭。
   * @n bit1 - 压卡传感器，0表示卡已被下压，1表示无卡。
   * @n bit2~bit7 - 分别表示从前端到后端的传感器，每个传感器值0表示有卡，值1表示无卡。
   * @return <0表示失败，==0表示成功，==1表示掉电无法使用。
   */
  API_ATTRIBUTE short USER_API dc_SelfServiceDeviceSensorStatus(HANDLE icdev, unsigned char *value);

  /**
   * @brief  设置弹卡模式。
   * @par    说明：
   * 设置弹卡模式。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 模式。
   * @n 0x00 - 弹到前端并夹卡。
   * @n 0x01 - 弹到后端。
   * @n 0x02 - 弹到前端不夹卡。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SelfServiceDeviceConfig(HANDLE icdev, unsigned char mode);

  /**
   * @brief  设置前端进卡模式。
   * @par    说明：
   * 设置前端进卡模式。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 模式。
   * @n 0x00 - 禁止进卡（无磁信号，无开关，无IC）。
   * @n 0x01 - 进卡（有磁信号，有开关，无IC）。
   * @n 0x02 - 进卡（无磁信号，有开关，无IC）。
   * @n 0x03 - 进卡（有磁信号，无开关，无IC）。
   * @n 0x04 - 进卡（无磁信号，无开关，有IC）。
   * @n 0x05 - 进卡（无磁信号，有开关，有IC）。
   * @n 0x06 - 进卡（有磁信号，无开关，有IC）。
   * @n 0x07 - 进卡（有磁信号，有开关，有IC）。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SelfServiceDeviceConfigFront(HANDLE icdev, unsigned char mode);

  /**
   * @brief  设置后端进卡模式。
   * @par    说明：
   * 设置后端进卡模式。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 模式。
   * @n 0x00 - 后端禁止进卡。
   * @n 0x01 - 后端磁卡方式进卡。
   * @n 0x02 - 后端非磁卡方式进卡。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SelfServiceDeviceConfigBack(HANDLE icdev, unsigned char mode);

  /**
   * @brief  设置停卡位置。
   * @par    说明：
   * 设置停卡位置。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 模式。
   * @n 0x00 - 前端不夹卡。
   * @n 0x01 - 前端夹卡。
   * @n 0x02 - 接触式IC卡位置。
   * @n 0x03 - 射频卡位置。
   * @n 0x04 - 磁卡位置。
   * @n 0x05 - 弹卡到后端。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SelfServiceDeviceConfigPlace(HANDLE icdev, unsigned char mode);

  /**
   * @brief  检测卡类型。
   * @par    说明：
   * 设备将自动移动卡片并且检测卡片类型。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败。
   * @n 0x00 - 表示无法检测到相应卡片。
   * @n 0x01 - 表示设备内无卡。
   * @n 0x11 - 表示Type A CPU Card。
   * @n 0x12 - 表示Type A 存储卡。
   * @n 0x13 - 表示Type A Mifare S50。
   * @n 0x14 - 表示Type A Mifare S70。
   * @n 0x15 - 表示Type A Mifare Ultralight。
   * @n 0x16 - 表示Type A Mifare MINI。
   * @n 0x18 - 表示Type A Ntag 21x / Ultralight EV1。
   * @n 0x1F - 表示Type A OTHERS MEM。
   * @n 0x21 - 表示Type B CPU Card。
   * @n 0x22 - 表示Type B 存储卡。
   * @n 0x31 - 表示接触T=0 CPU Card。
   * @n 0x32 - 表示接触T=1 CPU Card。
   * @n 0x41 - 表示4442 Card。
   * @n 0x42 - 表示4428 Card。
   */
  API_ATTRIBUTE short USER_API dc_SelfServiceDeviceCheckCardType(HANDLE icdev);

  /**
   * @brief  复位自助设备。
   * @par    说明：
   * 使自助设备进入上电初始状态，并且设置参数为缺省参数。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SelfServiceDeviceReset(HANDLE icdev);

  /**
   * @brief  写磁条卡参数设置。
   * @par    说明：
   * 写磁条卡参数设置。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 模式，0表示写三轨共移动三次，1表示写三轨共移动一次。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SelfServiceDeviceWriteMagConfig(HANDLE icdev, unsigned char mode);

  /**
   * @brief  读磁条卡参数设置。
   * @par    说明：
   * 读磁条卡参数设置。
   * @param[in] icdev 设备标识符。
   * @param[in] track1 1轨道参数。
   * @n bit0 - 轨道数据输出设置，0表示不输出，1表示输出。
   * @n bit1 - 轨道数据头设置，0表示无数据头，1表示有数据头。
   * @n bit2 - 轨道数据尾设置，0表示无数据尾，1表示有数据尾。
   * @n bit3~bit7 - 保留。
   * @param[in] track2 2轨道参数。
   * @n bit0 - 轨道数据输出设置，0表示不输出，1表示输出。
   * @n bit1 - 轨道数据头设置，0表示无数据头，1表示有数据头。
   * @n bit2 - 轨道数据尾设置，0表示无数据尾，1表示有数据尾。
   * @n bit3~bit7 - 保留。
   * @param[in] track3 3轨道参数。
   * @n bit0 - 轨道数据输出设置，0表示不输出，1表示输出。
   * @n bit1 - 轨道数据头设置，0表示无数据头，1表示有数据头。
   * @n bit2 - 轨道数据尾设置，0表示无数据尾，1表示有数据尾。
   * @n bit3~bit7 - 保留。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SelfServiceDeviceReadMagConfig(HANDLE icdev, unsigned char track1, unsigned char track2, unsigned char track3);

  /**
   * @brief  抖动模式设置。
   * @par    说明：
   * 抖动模式设置。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 模式，0表示抖动模式，1表示非抖动模式。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SelfServiceDeviceShakeConfig(HANDLE icdev, unsigned char mode);

  /**
   * @brief  前门电闸门设置。
   * @par    说明：
   * 前门电闸门设置。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 模式，0表示存在前门电闸门，1表示不存在前门电闸门。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SelfServiceDeviceSetFrontSwitch(HANDLE icdev, unsigned char mode);

  /**
   * @brief  前门电闸门获取。
   * @par    说明：
   * 前门电闸门获取。
   * @param[in] icdev 设备标识符。
   * @param[out] mode 模式，0表示存在前门电闸门，1表示不存在前门电闸门。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SelfServiceDeviceGetFrontSwitch(HANDLE icdev, unsigned char *mode);

  /**
   * @brief  错误恢复。
   * @par    说明：
   * 错误恢复。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类型，0x00表示异常卡前进卡处理，0x01表示异常卡后进卡处理。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SelfServiceDeviceRecoverError(HANDLE icdev, unsigned char type);

  /**
   * @brief  获取传感器状态。
   * @par    说明：
   * 获取设备传感器的状态。
   * @param[in] icdev 设备标识符。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据。
   * @n rdata[0] - 电闸门开关传感器，0x00表示打开，0x01表示关闭。
   * @n rdata[1] - 压卡传感器，0x00表示卡已被下压，0x01表示无卡。
   * @n rdata[2] - 入卡检测，0x00表示有，0x01表示无。
   * @n rdata[3] - 卡位1检测，0x00表示有，0x01表示无。
   * @n rdata[4] - 卡位2检测，0x00表示有，0x01表示无。
   * @n rdata[5] - 卡位3检测，0x00表示有，0x01表示无。
   * @n rdata[6] - 卡位4检测，0x00表示有，0x01表示无。
   * @n rdata[7] - 卡位5检测，0x00表示有，0x01表示无。
   * @n rdata[8] - IC卡检测，0x00表示有，0x01表示无。
   * @n rdata[9] - 步进电机检测，0x00表示下压，0x01表示升起。
   * @return <0表示失败，==0表示成功，==1表示掉电无法使用。
   */
  API_ATTRIBUTE short USER_API dc_SelfServiceDeviceSensorStatusEx(HANDLE icdev, int *rlen, unsigned char *rdata);

  /**
   * @brief  压住卡和放开卡。
   * @par    说明：
   * 压住卡和放开卡。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 模式，0x00表示放开卡片，0x01表示压住卡片。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SelfServiceDeviceKeepUpDown(HANDLE icdev, unsigned char mode);

  /**
   * @brief  写磁条卡。
   * @par    说明：
   * 写入数据到磁条卡中。
   * @param[in] icdev 设备标识符。
   * @param[in] t1_data 传入1轨道数据。
   * @param[in] t1_len 1轨道数据的长度，注意：-1表示清空此轨道数据，0表示忽略写此轨道。
   * @param[in] t2_data 传入2轨道数据。
   * @param[in] t2_len 2轨道数据的长度，注意：-1表示清空此轨道数据，0表示忽略写此轨道。
   * @param[in] t3_data 传入3轨道数据。
   * @param[in] t3_len 3轨道数据的长度，注意：-1表示清空此轨道数据，0表示忽略写此轨道。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SelfServiceDeviceWriteMag(HANDLE icdev, const unsigned char *t1_data, int t1_len, const unsigned char *t2_data, int t2_len, const unsigned char *t3_data, int t3_len);

  /**
   * @brief  检测多卡状态。
   * @par    说明：
   * 判断是否为多张卡，是否为Type A或Type B卡。
   * @param[in] icdev 设备标识符。
   * @param[out] type 卡类型，0x0A表示Type A卡，0x0B表示Type B卡。
   * @return <0表示失败，==0表示成功，==1表示无卡，==2表示存在多张卡。
   */
  API_ATTRIBUTE short USER_API dc_MulticardStatus(HANDLE icdev, unsigned char *type);

  /**
   * @brief  获取多个按键值。
   * @par    说明：
   * 获取设备键盘的按键值，每次调用可以获取多个按键值。
   * @param[in] icdev 设备标识符。
   * @param[in] ctime 设备超时值，单位为秒。
   * @param[out] rlen 返回按键数据的长度，不含'\0'。
   * @param[out] cpass 返回的按键数据，数据格式为'\0'结尾的字符串。
   * @return <0表示失败，==0表示成功，==0xA1表示取消输入，==0xA2表示输入超时。
   */
  API_ATTRIBUTE short USER_API dc_pass_key_value2(HANDLE icdev, unsigned char ctime, unsigned char *rlen, unsigned char *cpass);

  /**
   * @brief  读外部EEPROM。
   * @par    说明：
   * 读取设备外部EEPROM中的数据。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移地址。
   * @param[in] length 读取长度。
   * @param[out] data_buffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ReadOutEeprom(HANDLE icdev, int offset, int length, unsigned char *data_buffer);

  /**
   * @brief  写外部EEPROM。
   * @par    说明：
   * 写入数据到设备外部EEPROM中，可以用作数据保存等。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移地址。
   * @param[in] length 写入长度。
   * @param[in] data_buffer 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_WriteOutEeprom(HANDLE icdev, int offset, int length, const unsigned char *data_buffer);

  /**
   * @brief  读ID卡。
   * @par    说明：
   * 读取ID卡数据。
   * @param[in] icdev 设备标识符。
   * @param[in] time_ms 设备超时值，单位为毫秒。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ReadIdCardInfo(HANDLE icdev, int time_ms, int *rlen, unsigned char *rdata);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_cpy_getkey(HANDLE icdev, unsigned char *respkey, int *resplen);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_cpy_setkey(HANDLE icdev, unsigned char *genkey, int keylen);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_cpy_cpuapdu(HANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_cpy_cpuapdu_hex(HANDLE icdev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_cpy_cpuapduInt(HANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_cpy_cpuapduInt_hex(HANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_cpy_procommand(HANDLE icdev, unsigned char slen, unsigned char *sendbuffer, unsigned char *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_cpy_procommand_hex(HANDLE icdev, unsigned char slen, char *sendbuffer, unsigned char *rlen, char *databuffer, unsigned char timeout);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_cpy_procommandInt(HANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_cpy_procommandInt_hex(HANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer, unsigned char timeout);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * 对感应区CPU卡进行指令交互操作，注意此接口已封装卡协议部分。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @param[in] timeout 超时值，此值只在部分设备的底层使用，单位为250毫秒，一般调用建议值为7。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_procommandInt(HANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * ::dc_procommandInt 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_procommandInt_hex(HANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer, unsigned char timeout);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_ReadRfCfgReg(HANDLE icdev, int type, int number, int *value);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_WriteRfCfgReg(HANDLE icdev, int type, int number, int value);

  /**
   * @brief  初始化NFC。
   * @par    说明：
   * 切换射频到NFC点对点模式，操作NFC点对点模式之前必须使用此接口进行初始化。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_NfcInit(HANDLE icdev);

  /**
   * @brief  激活NFC。
   * @par    说明：
   * 寻NFC（即激活NFC后自动链接）并返回ATR数据。
   * @param[in] icdev 设备标识符。
   * @param[out] atr 返回的数据。
   * @param[out] atr_len 返回数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_NfcActivate(HANDLE icdev, unsigned char *atr, int *atr_len);

  /**
   * @brief  断开NFC。
   * @par    说明：
   * 断开链接并关闭天线，调用此接口必须先激活NFC。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_NfcDeactivate(HANDLE icdev);

  /**
   * @brief  NFC点对点数据透传（推送服务端）。
   * @par    说明：
   * 推送数据到NFC手机，调用此接口必须先激活NFC。
   * @param[in] icdev 设备标识符。
   * @param[in] request 发送数据。
   * @param[in] request_len 发送数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_NfcPush(HANDLE icdev, const unsigned char *request, int request_len);

  /**
   * @brief  NFC点对点数据透传（从服务端接收）。
   * @par    说明：
   * 设置接收模式，开启后将进入准备接收NFC手机数据的模式，调用此接口无须激活NFC。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 标志，0表示关闭，1表示开启。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_NfcService(HANDLE icdev, int flag);

  /**
   * @brief  接收NFC手机数据。
   * @par    说明：
   * 设备进入接收模式后，可以使用此接口来接收NFC手机的数据。
   * @param[in] icdev 设备标识符。
   * @param[out] response 返回的数据。
   * @param[out] response_len 返回数据的长度。
   * @param[in] time_ms 接口超时值，单位为秒。
   * @return <0表示失败，==0表示成功，==1表示手机不在感应区，==2表示数据需要重新传递。
   */
  API_ATTRIBUTE short USER_API dc_NfcFetch(HANDLE icdev, unsigned char *response, int *response_len, int time_ms);

  /**
   * @brief  接收NFC手机数据。
   * @par    说明：
   * 设备进入接收模式后，可以使用此接口来接收NFC手机的数据（CCID类设备使用）。
   * @param[in] icdev 设备标识符。
   * @param[in] time_ms 接口超时值，单位为秒。
   * @param[out] flag 标志，固定为1个字节，0x00表示中间帧，0x01表示起始帧，0x02表示结束帧，0x03表示起始结束帧。
   * @param[out] response 返回的数据。
   * @param[out] response_len 返回数据的长度。
   * @return <0表示失败，==0表示成功，==1表示手机不在感应区，==2表示数据需要重新传递。
   */
  API_ATTRIBUTE short USER_API dc_NfcDataReceiver(HANDLE icdev, int time_ms, unsigned char *flag, unsigned char *response, int *response_len);

  /**
   * @brief  NFC点对点数据透传（从服务端接收）。
   * @par    说明：
   * 设置接收模式，开启后将进入准备接收NFC手机数据的模式，调用此接口无须激活NFC。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 模式，0表示关闭，1表示开启。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_NfcSimulateCardConfig(HANDLE icdev, int mode);

  /**
   * @brief  磁条卡数据自动上传数据模式设置。
   * @par    说明：
   * 磁条卡数据自动上传数据模式设置。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 模式。
   * @n 0 - 输出数据中如果带字母，字母转化为大写字母。
   * @n 1 - 输出数据是原始数据（默认）。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_MagFormatConfig(HANDLE icdev, int mode);

  /**
   * @brief  获取通讯模块MAC地址。
   * @par    说明：
   * 获取通讯模块MAC地址。
   * @param[in] icdev 设备标识符。
   * @param[in] type 模块类型，1表示蓝牙模块，2表示WIFI模块，3表示GPRS/CDMA/全网通等模块。
   * @param[out] value 返回的MAC地址，数据格式为'\0'结尾的字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ReadModuleMacAddress(HANDLE icdev, int type, char *value);

  /**
   * @brief  设置通讯模块MAC地址。
   * @par    说明：
   * 设置通讯模块MAC地址。
   * @param[in] icdev 设备标识符。
   * @param[in] type 模块类型，1表示蓝牙模块，2表示WIFI模块，3表示GPRS/CDMA/全网通等模块。
   * @param[in] value 传入MAC地址，数据格式为'\0'结尾的字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_WriteModuleMacAddress(HANDLE icdev, int type, const char *value);

  /**
   * @brief  读取ATS数据。
   * @par    说明：
   * 读取Type A、Type B卡的ATS，此接口必须在卡片激活后才可使用。
   * @param[in] icdev 设备标识符。
   * @param[out] ats_len 返回ATS数据的长度。
   * @param[out] ats 返回的ATS数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ReadRfTypeABATS(HANDLE icdev, int *ats_len, unsigned char *ats);

  /**
   * @brief  读取ATS数据。
   * @par    说明：
   * ::dc_ReadRfTypeABATS 的HEX形式接口，参数 @a ats 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_ReadRfTypeABATS_hex(HANDLE icdev, int *ats_len, char *ats);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_RfFormatConfig(HANDLE icdev, int mode, int block_number, int offset, int length);

  /**
   * @brief  Flash扇区擦除。
   * @par    说明：
   * 擦除Flash扇区中的数据。
   * @param[in] icdev 设备标识符。
   * @param[in] sector 扇区地址，0xFFFF表示擦除全部。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_FlashErase(HANDLE icdev, int sector);

  /**
   * @brief  设备蜂鸣。
   * @par    说明：
   * 设备蜂鸣器按照指定参数进行蜂鸣，接口本身会直接返回，而不会等待蜂鸣器工作完成。
   * @param[in] icdev 设备标识符。
   * @param[in] beep_time_ms 单次蜂鸣时间，单位为毫秒。
   * @param[in] interval_time_ms 间隔时间，单位为毫秒。
   * @param[in] count 蜂鸣次数，其中0x0000表示停止，0xFFFF表示无限次。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_BuzzerBeep(HANDLE icdev, unsigned short beep_time_ms, unsigned short interval_time_ms, unsigned short count);

  /**
   * @brief  蓝牙模式设置。
   * @par    说明：
   * 设置蓝牙模块的通讯模式。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 模式，0x00表示串口模式（AT指令集或EEPROM读写），0x01表示蓝牙模式。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_BtModeControl(HANDLE icdev, unsigned char mode);

  /**
   * @brief  蓝牙波特率查询/设置。
   * @par    说明：
   * 查询/设置蓝牙模块的通讯波特率，修改的是蓝牙模块端。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 标志，0x00表示查询，0x01表示设置。
   * @param[in,out] value 波特率。
   * @n 0x00 - 9600。
   * @n 0x01 - 19200。
   * @n 0x02 - 38400。
   * @n 0x03 - 57600。
   * @n 0x04 - 115200。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_BtBaudrateControl(HANDLE icdev, unsigned char flag, unsigned char *value);

  /**
   * @brief  蓝牙连接密码查询/设置。
   * @par    说明：
   * 查询/设置蓝牙模块的连接密码。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 标志，0x00表示查询，0x01表示设置。
   * @param[in,out] value 密码字符串，查询时请至少分配64个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_BtPasswordControl(HANDLE icdev, unsigned char flag, char *value);

  /**
   * @brief  蓝牙模块地址查询/设置。
   * @par    说明：
   * 查询/设置蓝牙模块的地址。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 标志，0x00表示查询，0x01表示设置。
   * @param[in,out] length 地址的长度。
   * @param[in,out] value 地址，查询时请至少分配64个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_BtAddressControl(HANDLE icdev, unsigned char flag, unsigned char *length, unsigned char *value);

  /**
   * @brief  蓝牙模块名称查询/设置。
   * @par    说明：
   * 查询/设置蓝牙模块的名称。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 标志，0x00表示查询，0x01表示设置。
   * @param[in,out] value 名称字符串，查询时请至少分配64个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_BtNameControl(HANDLE icdev, unsigned char flag, char *value);

  /**
   * @brief  蓝牙用户自定义的配置命令。
   * @par    说明：
   * 采用读写EEPROM方式进行蓝牙模块的自定义配置。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 标志，0x00表示读EEPROM，0x01表示写EEPROM或AT指令集模式。
   * @param[in] offset 偏移地址。
   * @param[in] length 数据的长度。
   * @param[in,out] value 数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_BtUserControl(HANDLE icdev, unsigned char flag, int offset, int length, unsigned char *value);

  /**
   * @brief  蓝牙软件复位。
   * @par    说明：
   * 对蓝牙模块进行软件复位。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_BtReset(HANDLE icdev);

  /**
   * @brief  蓝牙模块通讯测试。
   * @par    说明：
   * 蓝牙模块在蓝牙通讯模式下，用此接口可以测试蓝牙通讯是否通畅，数据发送成功后，监测相关串口是否有接收到该数据来判断通讯是否成功。
   * @param[in] icdev 设备标识符。
   * @param[in] length 发送数据的长度。
   * @param[in] value 发送数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_BtCommunicationTest(HANDLE icdev, int length, const unsigned char *value);

  /**
   * @brief  蓝牙波特率查询/设置。
   * @par    说明：
   * 查询/设置与蓝牙模块之间的通讯波特率，修改的是设备端。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 标志，0x00表示查询，0x01表示设置。
   * @param[in,out] value 波特率。
   * @n 0x00 - 2400。
   * @n 0x01 - 4800。
   * @n 0x02 - 9600。
   * @n 0x03 - 14400。
   * @n 0x04 - 38400。
   * @n 0x05 - 57600。
   * @n 0x06 - 115200。
   * @n 0x07 - 256000。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_BtWithReaderBaudrateControl(HANDLE icdev, unsigned char flag, unsigned char *value);

  /**
   * @brief  蓝牙模块通讯。
   * @par    说明：
   * 与蓝牙模块进行通讯交互。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sdata 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_BtCommand(HANDLE icdev, int slen, const unsigned char *sdata, int *rlen, unsigned char *rdata);

  /**
   * @brief  获取蓝牙信号强度。
   * @par    说明：
   * 获取蓝牙信号强度。
   * @param[in] icdev 设备标识符。
   * @param[out] state 状态，0x00表示蓝牙未连接，0x01表示蓝牙已连接。
   * @param[out] value 信号值。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_BtSignalStrength(HANDLE icdev, unsigned char *state, short *value);

  /**
   * @brief  获取蓝牙响应。
   * @par    说明：
   * 获取蓝牙模块当前执行命令的响应或数据。
   * @param[in] icdev 设备标识符。
   * @param[out] length 返回数据的长度。
   * @param[out] value 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_BtDataResponse(HANDLE icdev, int *length, unsigned char *value);

  /**
   * @brief  蓝牙数据透传。
   * @par    说明：
   * 此接口用于将从蓝牙接收到的数据透传到外设中，并返回从外设接收的数据。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sdata 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_BtDataTransmit(HANDLE icdev, int slen, const unsigned char *sdata, int *rlen, unsigned char *rdata);

  /**
   * @brief  SAM_A模块指令交互。
   * @par    说明：
   * 此接口用于操作居民身份证验证安全控制模块。
   * @param[in] icdev 设备标识符。
   * @param[in] code 指令代码。
   * @n 0x10FF - 复位SAM_A，无发送数据，无返回数据。
   * @n 0x11FF - SAM_A状态检测，无发送数据，无返回数据。
   * @n 0x12FF - 读SAM_A管理信息，无发送数据，返回数据为16个字节的SAM_A编号。
   * @n 0x2001 - 寻找居民身份证，无发送数据，返回数据为4个字节的0x00。
   * @n 0x2002 - 选取居民身份证，无发送数据，返回数据为8个字节的0x00。
   * @n 0x3001 - 读机读文字信息和相片信息，无发送数据，返回数据格式为：【2个字节文字信息长度（Big-Endian）】【2个字节相片信息长度（Big-Endian）】【文字信息（不大于256个字节）】【相片信息（不大于1024个字节）】。
   * @n 0x3010 - 读机读文字信息、相片信息和指纹信息，无发送数据，返回数据格式为：【2个字节文字信息长度（Big-Endian）】【2个字节相片信息长度（Big-Endian）】【2个字节指纹信息长度（Big-Endian）】【文字信息（不大于256个字节）】【相片信息（不大于1024个字节）】【指纹信息（不大于1024个字节）】。
   * @n 0x3003 - 读追加住址信息，无发送数据，返回数据为70个字节的追加住址信息。
   * @n 0x6000 - 设置UART接口速率，速率为115200bps，无发送数据，无返回数据。
   * @n 0x6001 - 设置UART接口速率，速率为57600bps，无发送数据，无返回数据。
   * @n 0x6002 - 设置UART接口速率，速率为38400bps，无发送数据，无返回数据。
   * @n 0x6003 - 设置UART接口速率，速率为19200bps，无发送数据，无返回数据。
   * @n 0x6004 - 设置UART接口速率，速率为9600bps，无发送数据，无返回数据。
   * @n 0x61FF - 设置SAM_A与射频模块一帧通信数据的最大字节数，发送数据为1个字节的最大字节数，取值返回为0x18~0xFF，默认值为0x58，无返回数据。
   * @param[in] slen 发送数据的长度。
   * @param[in] sdata 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据，请至少分配4096个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SamAControl(HANDLE icdev, int code, int slen, const unsigned char *sdata, int *rlen, unsigned char *rdata);

  /**
   * @brief  读身份证。
   * @par    说明：
   * 读取身份证的原始信息数据。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类型。
   * @n 1 - 读取文字信息、相片信息和指纹信息。
   * @n 2 - 读取追加住址信息。
   * @n 3 - 读取文字信息、相片信息、指纹信息和追加住址信息。
   * @param[out] text_len 返回文字信息的长度。
   * @param[out] text 返回的文字信息，请至少分配256个字节。
   * @param[out] photo_len 返回相片信息的长度。
   * @param[out] photo 返回的相片信息，请至少分配1024个字节。
   * @param[out] fingerprint_len 返回指纹信息的长度。
   * @param[out] fingerprint 返回的指纹信息，请至少分配1024个字节。
   * @param[out] extra_len 返回追加住址信息的长度。
   * @param[out] extra 返回的追加住址信息，请至少分配70个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SamAReadCardInfo(HANDLE icdev, int type, int *text_len, unsigned char *text, int *photo_len, unsigned char *photo, int *fingerprint_len, unsigned char *fingerprint, int *extra_len, unsigned char *extra);

  /**
   * @brief  获取身份证卡片种类。
   * @par    说明：
   * 获取身份证卡片种类。
   * @param[in] icdev 设备标识符。
   * @param[in] info_len 文字信息的长度。
   * @param[in] info 文字信息。
   * @return <0表示失败，==0表示中国人居民身份证，==1表示外国人永久居留证（2017版），==2表示港澳台居民居住证，==3表示外国人永久居留证（新版）。
   */
  API_ATTRIBUTE short USER_API dc_GetIdCardType(HANDLE icdev, int info_len, const unsigned char *info);

  /**
   * @brief  解析文字信息。
   * @par    说明：
   * 解析中国人居民身份证文字信息，获取相应的条目。
   * @param[in] icdev 设备标识符。
   * @param[in] charset 获取条目将采用的字符集，0表示GBK，1表示UCS-2LE，2表示UTF-8。
   * @param[in] info_len 文字信息的长度。
   * @param[in] info 文字信息。
   * @param[out] name 姓名，请至少分配64个字节。
   * @param[out] sex 性别代码，请至少分配8个字节。
   * @param[out] nation 民族，请至少分配12个字节。
   * @param[out] birth_day 出生日期，请至少分配36个字节。
   * @param[out] address 住址，请至少分配144个字节。
   * @param[out] id_number 公民身份号码，请至少分配76个字节。
   * @param[out] department 签发机关，请至少分配64个字节。
   * @param[out] expire_start_day 证件签发日期，请至少分配36个字节。
   * @param[out] expire_end_day 证件终止日期，请至少分配36个字节。
   * @param[out] reserved 预留项，请至少分配76个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ParseTextInfo(HANDLE icdev, int charset, int info_len, const unsigned char *info, unsigned char *name, unsigned char *sex, unsigned char *nation, unsigned char *birth_day, unsigned char *address, unsigned char *id_number, unsigned char *department, unsigned char *expire_start_day, unsigned char *expire_end_day, unsigned char *reserved);

  /**
   * @brief  解析文字信息。
   * @par    说明：
   * 解析外国人永久居留证（2017版）文字信息，获取相应的条目。
   * @param[in] icdev 设备标识符。
   * @param[in] charset 获取条目将采用的字符集，0表示GBK，1表示UCS-2LE，2表示UTF-8。
   * @param[in] info_len 文字信息的长度。
   * @param[in] info 文字信息。
   * @param[out] english_name 英文姓名，请至少分配244个字节。
   * @param[out] sex 性别代码，请至少分配8个字节。
   * @param[out] id_number 永久居留证号码，请至少分配64个字节。
   * @param[out] citizenship 国籍或所在地区代码，请至少分配16个字节。
   * @param[out] chinese_name 中文姓名，请至少分配64个字节。
   * @param[out] expire_start_day 证件签发日期，请至少分配36个字节。
   * @param[out] expire_end_day 证件终止日期，请至少分配36个字节。
   * @param[out] birth_day 出生日期，请至少分配36个字节。
   * @param[out] version_number 证件版本号，请至少分配12个字节。
   * @param[out] department_code 当次申请受理机关代码，请至少分配20个字节。
   * @param[out] type_sign 证件类型标识，请至少分配8个字节。
   * @param[out] reserved 预留项，请至少分配16个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ParseTextInfoForForeigner(HANDLE icdev, int charset, int info_len, const unsigned char *info, unsigned char *english_name, unsigned char *sex, unsigned char *id_number, unsigned char *citizenship, unsigned char *chinese_name, unsigned char *expire_start_day, unsigned char *expire_end_day, unsigned char *birth_day, unsigned char *version_number, unsigned char *department_code, unsigned char *type_sign, unsigned char *reserved);

  /**
   * @brief  解析文字信息。
   * @par    说明：
   * 解析港澳台居民居住证文字信息，获取相应的条目。
   * @param[in] icdev 设备标识符。
   * @param[in] charset 获取条目将采用的字符集，0表示GBK，1表示UCS-2LE，2表示UTF-8。
   * @param[in] info_len 文字信息的长度。
   * @param[in] info 文字信息。
   * @param[out] name 姓名，请至少分配64个字节。
   * @param[out] sex 性别代码，请至少分配8个字节。
   * @param[out] reserved 预留项，请至少分配12个字节。
   * @param[out] birth_day 出生日期，请至少分配36个字节。
   * @param[out] address 住址，请至少分配144个字节。
   * @param[out] id_number 公民身份号码，请至少分配76个字节。
   * @param[out] department 签发机关，请至少分配64个字节。
   * @param[out] expire_start_day 证件签发日期，请至少分配36个字节。
   * @param[out] expire_end_day 证件终止日期，请至少分配36个字节。
   * @param[out] pass_number 通行证号码，请至少分配40个字节。
   * @param[out] sign_count 签发次数，请至少分配12个字节。
   * @param[out] reserved2 预留项，请至少分配16个字节。
   * @param[out] type_sign 证件类型标识，请至少分配8个字节。
   * @param[out] reserved3 预留项，请至少分配16个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ParseTextInfoForHkMoTw(HANDLE icdev, int charset, int info_len, const unsigned char *info, unsigned char *name, unsigned char *sex, unsigned char *reserved, unsigned char *birth_day, unsigned char *address, unsigned char *id_number, unsigned char *department, unsigned char *expire_start_day, unsigned char *expire_end_day, unsigned char *pass_number, unsigned char *sign_count, unsigned char *reserved2, unsigned char *type_sign, unsigned char *reserved3);

  /**
   * @brief  解析文字信息。
   * @par    说明：
   * 解析外国人永久居留证（新版）文字信息，获取相应的条目。
   * @param[in] icdev 设备标识符。
   * @param[in] charset 获取条目将采用的字符集，0表示GBK，1表示UCS-2LE，2表示UTF-8。
   * @param[in] info_len 文字信息的长度。
   * @param[in] info 文字信息。
   * @param[out] chinese_name 中文姓名，请至少分配64个字节。
   * @param[out] sex 性别代码，请至少分配8个字节。
   * @param[out] renew_count 换证次数，请至少分配12个字节。
   * @param[out] birth_day 出生日期，请至少分配36个字节。
   * @param[out] english_name 英文姓名，请至少分配144个字节。
   * @param[out] id_number 证件号码，请至少分配76个字节。
   * @param[out] reserved 预留项，请至少分配64个字节。
   * @param[out] expire_start_day 有效期起始日期，请至少分配36个字节。
   * @param[out] expire_end_day 有效期截止日期，请至少分配36个字节。
   * @param[out] english_name_ex 英文姓名备用，请至少分配48个字节。
   * @param[out] citizenship 国籍代码，请至少分配16个字节。
   * @param[out] type_sign 证件类型标识，请至少分配8个字节。
   * @param[out] prev_related_info 既往版本外国人永久居留证件号码关联项，请至少分配16个字节。
   * @param[out] old_id_number 旧永久居留证号码，请至少分配64个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ParseTextInfoForNewForeigner(HANDLE icdev, int charset, int info_len, const unsigned char *info, unsigned char *chinese_name, unsigned char *sex, unsigned char *renew_count, unsigned char *birth_day, unsigned char *english_name, unsigned char *id_number, unsigned char *reserved, unsigned char *expire_start_day, unsigned char *expire_end_day, unsigned char *english_name_ex, unsigned char *citizenship, unsigned char *type_sign, unsigned char *prev_related_info, unsigned char *old_id_number);

  /**
   * @brief  解析相片信息。
   * @par    说明：
   * 解析相片信息，通过公安部相片解码库还原相片图像数据。
   * @param[in] icdev 设备标识符。
   * @param[in] type 相片图像数据的格式，0表示BMP文件，1表示BMP缓存，2表示BMP Base64字符串。
   * @param[in] info_len 相片信息的长度。
   * @param[in] info 相片信息。
   * @param[in,out] photo_len 数据的长度。
   * @n BMP文件 - 无效。
   * @n BMP缓存 - 传入 @a photo 分配的字节数，返回相片图像数据的长度。
   * @n BMP Base64字符串 - 传入 @a photo 分配的字节数，返回相片图像数据Base64字符串的长度，不含'\0'。
   * @param[in,out] photo 数据。
   * @n BMP文件 - 传入文件名，请确保有写入的权限。
   * @n BMP缓存 - 返回的相片图像数据，请至少分配65536个字节。
   * @n BMP Base64字符串 - 返回的相片图像数据Base64字符串，请至少分配65536个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ParsePhotoInfo(HANDLE icdev, int type, int info_len, const unsigned char *info, int *photo_len, unsigned char *photo);

  /**
   * @brief  解析其它信息。
   * @par    说明：
   * 解析其它信息。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 标志。
   * @n 当信息采用GBK编码时，使用0x00表示性别，0x01表示民族，0x02表示国籍。
   * @n 当信息采用UCS-2LE编码时，使用0x10表示性别，0x11表示民族，0x12表示国籍。
   * @n 当信息采用UTF-8编码时，使用0x20表示性别，0x21表示民族，0x22表示国籍。
   * @param[in] in_info 传入信息。
   * @param[out] out_info 返回的信息。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ParseOtherInfo(HANDLE icdev, int flag, const unsigned char *in_info, unsigned char *out_info);

  /**
   * @brief  检测接触式CPU卡。
   * @par    说明：
   * 检测当前卡座中CPU卡的状态。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败。
   * @n 0 - 状态未知。
   * @n 1 - 无卡。
   * @n 2 - 有卡，但卡类型未知。
   * @n 3 - 有卡，卡未上电。
   * @n 4 - 有卡，卡已上电。
   */
  API_ATTRIBUTE short USER_API dc_CpuCardStatus(HANDLE icdev);

  /**
   * @brief  唤醒设备。
   * @par    说明：
   * 唤醒设备，此接口用于向设备发出唤醒请求，成功则表示设备已经唤醒。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_WakeDevice(HANDLE icdev);

  /**
   * @brief  休眠设备。
   * @par    说明：
   * 休眠设备，此接口用于向设备发出休眠请求，执行成功仅仅表示成功发出休眠请求，并不表示设备已经进入休眠状态。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_RestDevice(HANDLE icdev);

  /**
   * @brief  获取电池电量。
   * @par    说明：
   * 获取电池电量。
   * @param[in] icdev 设备标识符。
   * @param[out] value 电池百分比（0x00~0x64），0x00表示0%，0x64表示100%。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetBatteryValue(HANDLE icdev, unsigned char *value);

  /**
   * @brief  防卡冲突。
   * @par    说明：
   * 支持ISO 14443 Type A类型卡片的防卡冲突。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 模式，0x93表示一级，0x95表示二级，0x97表示三级。
   * @param[in] _Bcnt 保留，固定为0x00。
   * @param[out] _Snr 返回的卡序列号。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_anticoll_mode(HANDLE icdev, unsigned char mode, unsigned char _Bcnt, unsigned int *_Snr);

  /**
   * @brief  选卡操作。
   * @par    说明：
   * 通过指定序列号，选取相应的卡片。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 模式，0x93表示一级，0x95表示二级，0x97表示三级。
   * @param[in] _Snr 卡片序列号。
   * @param[out] _Size 返回的SAK值。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_select_mode(HANDLE icdev, unsigned char mode, unsigned int _Snr, unsigned char *_Size);

  /**
   * @brief  激活卡片。
   * @par    说明：
   * 支持ISO 14443 Type B类型卡片的激活。
   * @param[in] icdev 设备标识符。
   * @param[in] PUPI 伪唯一PICC标识符。
   * @param[in] CID 信道号。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_attribInt(HANDLE icdev, unsigned char *PUPI, unsigned char CID);

  /**
   * @brief  卡上电。
   * @par    说明：
   * 对指定卡座的卡片进行上电操作。
   * @param[in] icdev 设备标识符。
   * @param[in] icc_slot_no 卡座编号，用户卡为0x0n，SAM卡为0x1n，其中“n”的取值范围为1~F。
   * @param[out] response 返回的复位信息，请至少分配128个字节。
   * @return <0表示失败，否则为返回复位信息的长度。
   * @n -1 - 卡片类型不对。
   * @n -2 - 无卡。
   * @n -3 - 有卡未上电。
   * @n -4 - 卡片无应答。
   */
  API_ATTRIBUTE short USER_API dc_rf_poweron(HANDLE icdev, unsigned char icc_slot_no, unsigned char *response);

  /**
   * @brief  卡下电。
   * @par    说明：
   * 对指定卡座的卡片进行下电操作。
   * @param[in] icdev 设备标识符。
   * @param[in] icc_slot_no 卡座编号，用户卡为0x0n，SAM卡为0x1n，其中“n”的取值范围为1~F。
   * @return <0表示失败，==0表示成功。
   * @n -1 - 卡片类型不对。
   * @n -2 - 无卡。
   * @n -3 - 有卡未上电。
   * @n -4 - 卡片无应答。
   */
  API_ATTRIBUTE short USER_API dc_rf_poweroff(HANDLE icdev, unsigned char icc_slot_no);

  /**
   * @brief  获取卡状态。
   * @par    说明：
   * 获取指定卡座的卡片状态。
   * @param[in] icdev 设备标识符。
   * @param[in] icc_slot_no 卡座编号，用户卡为0x0n，SAM卡为0x1n，其中“n”的取值范围为1~F。
   * @return <0表示失败，==0表示有卡且已上电。
   * @n -1 - 卡片类型不对。
   * @n -2 - 无卡。
   * @n -3 - 有卡未上电。
   * @n -4 - 卡片无应答。
   */
  API_ATTRIBUTE short USER_API dc_rf_getstatus(HANDLE icdev, unsigned char icc_slot_no);

  /**
   * @brief  卡指令交互。
   * @par    说明：
   * 对指定卡座的卡片进行指令交互操作。
   * @param[in] icdev 设备标识符。
   * @param[in] icc_slot_no 卡座编号，用户卡为0x0n，SAM卡为0x1n，其中“n”的取值范围为1~F。
   * @param[in] length_of_command_apdu 发送数据的长度。
   * @param[in] command_apdu 发送数据。
   * @param[out] response_apdu 返回的数据。
   * @return <0表示失败，否则为返回数据的长度。
   * @n -1 - 卡片类型不对。
   * @n -2 - 无卡。
   * @n -3 - 有卡未上电。
   * @n -4 - 卡片无应答。
   */
  API_ATTRIBUTE short USER_API dc_rf_application(HANDLE icdev, unsigned char icc_slot_no, int length_of_command_apdu, unsigned char *command_apdu, unsigned char *response_apdu);

  /**
   * @brief  获取多张Type A卡UID。
   * @par    说明：
   * 获取多张Type A卡UID。
   * @param[in] icdev 设备标识符。
   * @param[out] uid_data 返回的数据，请至少分配256个字节。格式为：【1字节UID条目数量】【1字节UID长度】【UID】【1字节UID2长度】【UID2】...。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_RequestMultiCard(HANDLE icdev, unsigned char *uid_data);

  /**
   * @brief  选择一张Type A卡并激活。
   * @par    说明：
   * 选择一张Type A卡并激活。
   * @param[in] icdev 设备标识符。
   * @param[in] uid_len 传入UID的长度。
   * @param[in] uid 传入UID。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ActivateCard(HANDLE icdev, unsigned char uid_len, const unsigned char *uid);

  /**
   * @brief  获取最后响应码。
   * @par    说明：
   * 获取最后一次和设备交互时设备返回的响应状态代码，此接口获取的响应码依赖设备内部固件代码的实现，不同的设备可能有不一致的响应码。
   * @param[in] icdev 设备标识符。
   * @param[out] code 返回的响应码。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetLastResponseCode(HANDLE icdev, unsigned int *code);

  /**
   * @brief  多个Flash扇区擦除。
   * @par    说明：
   * 擦除指定Flash扇区中的数据。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类型，0x00表示核心板上的Flash，0x01表示主Spi的Flash，0x02表示备份Spi的Flash，0x03表示语音存储的Flash，0x04表示字库存储的Flash。
   * @param[in] sector 扇区地址，0xFFFFFFFF表示擦除全部。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_MultiFlashErase(HANDLE icdev, unsigned char type, unsigned int sector);

  /**
   * @brief  多个Flash读数据。
   * @par    说明：
   * 读取指定Flash中的数据。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类型，0x00表示核心板上的Flash，0x01表示主Spi的Flash，0x02表示备份Spi的Flash，0x03表示语音存储的Flash，0x04表示字库存储的Flash。
   * @param[in] offset_h 偏移地址（高位）。
   * @param[in] offset_l 偏移地址（低位）。
   * @param[in] length_h 读取长度（高位）。
   * @param[in] length_l 读取长度（低位）。
   * @param[out] data_buffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_MultiFlashRead(HANDLE icdev, unsigned char type, unsigned int offset_h, unsigned int offset_l, unsigned int length_h, unsigned int length_l, unsigned char *data_buffer);

  /**
   * @brief  多个Flash写数据。
   * @par    说明：
   * 写入数据到指定Flash中。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类型，0x00表示核心板上的Flash，0x01表示主Spi的Flash，0x02表示备份Spi的Flash，0x03表示语音存储的Flash，0x04表示字库存储的Flash。
   * @param[in] offset_h 偏移地址（高位）。
   * @param[in] offset_l 偏移地址（低位）。
   * @param[in] length_h 读取长度（高位）。
   * @param[in] length_l 读取长度（低位）。
   * @param[in] data_buffer 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_MultiFlashWrite(HANDLE icdev, unsigned char type, unsigned int offset_h, unsigned int offset_l, unsigned int length_h, unsigned int length_l, const unsigned char *data_buffer);

  /**
   * @brief  多个FLASH容量大小获取。
   * @par    说明：
   * 获取指定Flash的容量大小。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类型，0x00表示核心板上的Flash，0x01表示主Spi的Flash，0x02表示备份Spi的Flash，0x03表示语音存储的Flash，0x04表示字库存储的Flash。
   * @param[out] length_h 返回的长度（高位）。
   * @param[out] length_l 返回的长度（低位）。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_MultiFlashGetSize(HANDLE icdev, unsigned char type, unsigned int *length_h, unsigned int *length_l);

  /**
   * @brief  多重寻卡。
   * @par    说明：
   * 有卡寻卡、无卡寻卡、多卡防冲突。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败。
   * @n 0 - 操作成功。
   * @n 1 - 未检测到射频卡。
   * @n 2 - 检测到身份证。
   * @n 3 - 多卡报错。
   * @n 4 - 其它射频卡。
   * @n 5 - 检测到Type A / Type B CPU卡。
   * @n 6 - 检测到有卡后的卡片还在感应区。
   */
  API_ATTRIBUTE short USER_API dc_RfMultiRequest(HANDLE icdev);

  /**
   * @brief  TTS语音播放。
   * @par    说明：
   * TTS语音播放。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类型，0x00表示停止播放，0x01表示播放内容为UCS-2LE编码，0x02表示播放内容为GBK编码。
   * @param[in] content 播放内容。
   * @param[in] content_len 播放内容的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_TtsVoicePlay(HANDLE icdev, unsigned char type, const unsigned char *content, int content_len);

  /**
   * @brief  TTS语音设置。
   * @par    说明：
   * TTS语音设置。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类型，0x01表示设置语速，0x02表示设置音量，0x03表示设置音色。
   * @param[in] value 增益值（0x00~0xFF），正常为0x80。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_TtsVoiceConfig(HANDLE icdev, unsigned char type, unsigned char value);

  /**
   * @brief  配置二维码工作模式。
   * @par    说明：
   * 配置二维码工作模式。
   * @param[in] icdev 设备标识符。
   * @param[in] content 配置内容。
   * @param[in] content_len 配置内容的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_Config2DBarcode(HANDLE icdev, const unsigned char *content, int content_len);

  /**
   * @brief  扫描二维码。
   * @par    说明：
   * 扫描二维码。
   * @param[in] icdev 设备标识符。
   * @param[in] time_ms 设备超时值，单位为毫秒。
   * @param[out] message_data 二维码，数据格式为'\0'结尾的字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_Scan2DBarcode(HANDLE icdev, unsigned short time_ms, char *message_data);

  /**
   * @brief  打开密码键盘。
   * @par    说明：
   * 打开密码键盘并且分配相关资源。
   * @param[in] icdev 设备标识符。
   * @param[in] number 编号，0表示内置密码键盘，1表示外接密码键盘（Android方向），2表示外接密码键盘（P3方向）。
   * @param[out] version 返回的版本字符串，请至少分配128个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_KeypadOpen(HANDLE icdev, int number, char *version);

  /**
   * @brief  关闭密码键盘。
   * @par    说明：
   * 关闭密码键盘并且释放相关资源。
   * @param[in] icdev 设备标识符。
   * @param[in] number 编号，0表示内置密码键盘，1表示外接密码键盘。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_KeypadClose(HANDLE icdev, int number);

  /**
   * @brief  装载密钥。
   * @par    说明：
   * 装载密钥到密码键盘中永久保存。
   * @param[in] icdev 设备标识符。
   * @param[in] number 编号，0表示内置密码键盘，1表示外接密码键盘。
   * @param[in] set_index 密钥套索引（0~15）。
   * @param[in] sub_index 密钥索引（0~15），其中0表示主密钥。
   * @param[in] type 密钥类型，0x00表示单DES，0x01表示三DES，0x02表示24字节三DES，0x10表示SM4，0x20表示AES128，0x21表示AES192，0x22表示AES256。
   * @param[in] mode 装载模式，0x00表示明文装载，0x01表示密文装载。
   * @param[in] key_data 密钥数据。
   * @param[in] key_len 密钥数据的长度。
   * @param[in] flag 标志，0表示不验证，1表示验证类别1，2表示验证类别2。
   * @param[in,out] ex_data 附加数据，含义由 @a flag 的值来决定。
   * @n flag为1 - ex_data为out形式的参数，返回加密全0x00后的数据。
   * @n flag为2 - ex_data为In形式的参数，传入加密全0x00后的数据。
   * @param[in,out] ex_len 附加数据的长度，含义由 @a flag 的值来决定。
   * @n flag为1 - ex_len为Out形式的参数，返回 @a ex_data 的长度。
   * @n flag为2 - ex_len为In形式的参数，传入 @a ex_data 的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_KeypadLoadKey(HANDLE icdev, int number, int set_index, int sub_index, int type, int mode, const unsigned char *key_data, int key_len, int flag, unsigned char *ex_data, int *ex_len);

  /**
   * @brief  清除密钥。
   * @par    说明：
   * 清除密码键盘中指定的密钥。
   * @param[in] icdev 设备标识符。
   * @param[in] number 编号，0表示内置密码键盘，1表示外接密码键盘。
   * @param[in] set_index 密钥套索引（0~15），-1表示全部密钥套。
   * @param[in] sub_index 密钥索引（0~15），其中0表示主密钥，-1表示全部密钥。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_KeypadClearKey(HANDLE icdev, int number, int set_index, int sub_index);

  /**
   * @brief  获取密钥类型。
   * @par    说明：
   * 获取密码键盘中指定密钥的类型。
   * @param[in] icdev 设备标识符。
   * @param[in] number 编号，0表示内置密码键盘，1表示外接密码键盘。
   * @param[in] set_index 密钥套索引（0~15）。
   * @param[in] sub_index 密钥索引（0~15），其中0表示主密钥。
   * @param[out] type 密钥类型，0x00表示单DES，0x01表示三DES，0x02表示24字节三DES，0x10表示SM4，0x20表示AES128，0x21表示AES192，0x22表示AES256。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_KeypadGetKeyType(HANDLE icdev, int number, int set_index, int sub_index, int *type);

  /**
   * @brief  数据加解密。
   * @par    说明：
   * 使用密码键盘中装载的密钥进行数据加解密操作。
   * @param[in] icdev 设备标识符。
   * @param[in] number 编号，0表示内置密码键盘，1表示外接密码键盘。
   * @param[in] set_index 密钥套索引（0~15）。
   * @param[in] sub_index 密钥索引（0~15），其中0表示主密钥。
   * @param[in] flag 标志，0x00表示使用密钥默认算法，0x01表示使用密钥0~7字节算单DES，0x02表示使用密钥8~15字节算单DES，0x03表示使用密钥16~23字节算单DES。
   * @param[in] mode 模式，0x00表示加密，0x01表示解密，0x10表示ECB加密，0x11表示ECB解密，0x20表示CBC加密，0x21表示CBC解密。
   * @param[in] in_data 输入数据。
   * @param[in] in_len 输入数据的长度。
   * @n 当mode为0x00或0x01时，in_data为加密或解密前的数据。
   * @n 当mode为0x10或0x11时，in_data数据格式为：【4字节序号】【数据块】，序号高位在前，为0时数据块为空，从1开始递增传递数据块序号，如果是最后一块，那么序号为0xFFFFFFFF，数据块小于等于设备返回的最大块长度。
   * @n 当mode为0x20或0x21时，in_data数据格式为：【4字节序号】【IV值/数据块】，序号高位在前，为0时传递IV值，从1开始递增传递数据块序号，如果是最后一块，那么序号为0xFFFFFFFF，数据块小于等于设备返回的最大块长度。
   * @param[out] out_data 输出数据。
   * @param[out] out_len 输出数据的长度。
   * @n 当mode为0x00或0x01时，out_data为加密或解密后的数据。
   * @n 当mode为0x10或0x11或0x20或0x21时，out_data数据格式为：【4字节序号】【最大块长度/数据块】，序号高位在前，为0时返回最大块长度（大于等于8并且小于等于1024），高位在前，否则返回本次加密或解密后的数据块。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_KeypadAlgorithm(HANDLE icdev, int number, int set_index, int sub_index, int flag, int mode, const unsigned char *in_data, int in_len, unsigned char *out_data, int *out_len);

  /**
   * @brief  启动按键输入。
   * @par    说明：
   * 启动密码键盘的按键输入，这时设备将开始接收用户的按键输入操作。注意：如果之前已经启动并未退出，那么再次调用将会使用新的参数来更新内部相关属性。
   * @param[in] icdev 设备标识符。
   * @param[in] number 编号，0表示内置密码键盘，1表示外接密码键盘。
   * @param[in] mode 模式。
   * @n 0 - 表示获取明文。
   * @n 1 - 表示获取ANSI X9.8 PIN BLOCK加密的按键数据，此模式仅支持块长度为8的数据，如需使用SM4、AES等算法计算PIN BLOCK，建议使用模式6并且自己组装需要异或的数据。
   * @n 2 - 表示获取IBM3624加密的按键数据。
   * @n 3 - 表示获取ASCII加密的按键数据。
   * @n 4 - 表示使用金额模式。
   * @n 5 - 表示使用ISO5964-1标准的Format4格式。
   * @n 6 - 表示获取ANSI X9.8 PIN BLOCK加密的按键数据。
   * @param[in] set_index 密钥套索引（0~15），明文模式内部将忽略它的传入。
   * @param[in] sub_index 密钥索引（0~15），其中0表示主密钥，明文模式内部将忽略它的传入。
   * @param[in] in_data 输入数据，依赖于模式，明文模式内部将忽略它的传入，密文模式为卡号，固定为12个字节。
   * @param[in] in_len 输入数据的长度。
   * @param[in] min_len 最小长度。
   * @param[in] max_len 最大长度。
   * @param[in] auto_end 按键时结束控制（此处的结束表示的仅仅是设备是否会自动加入【确认】键值），0表示不自动结束，1表示自动结束。
   * @param[in] enable_option 控制选项，每个bit控制一种选项的关闭或启用。
   * @n bit0 - 按键时蜂鸣控制，0表示关闭，1表示启用。
   * @n bit1 - 开始接收按键时语音提示控制，0表示关闭，1表示启用。
   * @n bit2 - 获取明文模式下设备隐藏屏幕密码显示控制，0表示关闭，1表示启用。
   * @param[in] time_s 从开始按键后，两次按键之间的时间间隔超时值，单位为秒，其中-1表示不使用时间间隔超时值，0表示缺省值（由设备实现决定）。
   * @param[out] out_data 输出数据。
   * @param[out] out_len 输出数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_KeypadStartInput(HANDLE icdev, int number, int mode, int set_index, int sub_index, const unsigned char *in_data, int in_len, int min_len, int max_len, int auto_end, int enable_option, int time_s, unsigned char *out_data, int *out_len);

  /**
   * @brief  获取键值。
   * @par    说明：
   * 获取一个按下的按键值，每次只能获取一个。<br>
   * 关于按键内部原理说明：<br>
   * -# 设备内部定义有键值缓存区和数值缓存区，键值缓存区保存所有键值，是为了上层可以依次取出来用以支持输入状态的显示，数值缓存区只保存数字键值。
   * -# 启动按键输入后，数值缓存区、键值缓存区同时保存数据，本接口调用时设备发送键值缓存区键值，如没有按键设备生成键值0xFF并发送（注意本接口此时依然返回成功），如果在指定时间内没有后续按键，设备将生成键值0x80并发送。注意这两种情况下，都可以继续获得后续按键。
   * -# 数值缓冲区的数量小于最小长度时候，【确认】键不响应（按键不叫）。
   * -# 数值缓冲区的数量大于最大长度时候，数字按键不响应（按键不叫），功能键还可以正常响应。
   * -# 当使用自动结束时，数值缓冲区的数量等于最大长度的时候，设备自动生成键值0x0D并发送。
   * <table>
   * <tr><th>【退格】键   <td>值为0x08，按下此键后，如果数值缓冲区的数量大于0，那么数量将减1。
   * <tr><th>【确认】键   <td>值为0x0D，当按下此键或设备自动生成键值0x0D后，所有按键都不再响应（按键不叫），本接口获取到此键值后不应继续调用本接口，否则结果未知。
   * <tr><th>【取消】键   <td>值为0x1B，按下此键后，数值缓冲区清空，所有按键都不再响应（按键不叫），本接口获取到此键值后不应继续调用本接口，否则结果未知。
   * <tr><th>【清除】键   <td>值为0x7F，按下此键后，数值缓冲区清空，继续
   * </table>
   * @param[in] icdev 设备标识符。
   * @param[in] number 编号，0表示内置密码键盘，1表示外接密码键盘。
   * @param[out] value 明文输入模式下数值键为键值的ASCII值，如键盘1为0x31；密文输入模式下数值键为0x2A；任何输入模式下控制键的键值一致，【退格】为0x08，【确认】为0x0D，【取消】为0x1B，【清除】为0x7F；另外没有按键按下为0xFF，输入超时为0x80。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_KeypadGetKeyValue(HANDLE icdev, int number, unsigned char *value);

  /**
   * @brief  退出并且获取密码数据。
   * @par    说明：
   * 退出并且获取密码键盘的密码数据，这时设备将停止接收用户的按键输入操作。
   * @param[in] icdev 设备标识符。
   * @param[in] number 编号，0表示内置密码键盘，1表示外接密码键盘。
   * @param[in] mode 模式，0表示仅退出，1表示退出并且获取数据。
   * @param[out] out_data 输出数据。
   * @param[out] out_len 输出数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_KeypadExitAndGetInput(HANDLE icdev, int number, int mode, unsigned char *out_data, int *out_len);

  /**
   * @brief  设置键值。
   * @par    说明：
   * 设置一个按下的按键值，每次只能设置一个。
   * @param[in] icdev 设备标识符。
   * @param[in] number 编号，0表示内置密码键盘，1表示外接密码键盘。
   * @param[in] value 数值键为键值的ASCII值，如键盘1为0x31；控制键【退格】为0x08，【确认】为0x0D，【取消】为0x1B，【清除】为0x7F。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_KeypadSetKeyValue(HANDLE icdev, int number, unsigned char value);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_KeypadCalculatePinBlock(HANDLE icdev, int number, int mode, int set_index, int sub_index, const unsigned char *card_number_data, int card_number_len, const unsigned char *in_data, int in_len, unsigned char *out_data, int *out_len);

  /**
   * @brief  获取设备唯一识别码。
   * @par    说明：
   * 获取设备内部写入的唯一识别码。
   * @param[in] icdev 设备标识符。
   * @param[out] uid 返回的唯一识别码字符串，请至少分配33个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetDeviceUid(HANDLE icdev, char *uid);

  /**
   * @brief  获取设备商户号。
   * @par    说明：
   * 获取设备内部写入的商户号。
   * @param[in] icdev 设备标识符。
   * @param[out] number 返回的商户号字符串，请至少分配33个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetDeviceTradeNumber(HANDLE icdev, char *number);

  /**
   * @brief  获取设备终端号。
   * @par    说明：
   * 获取设备内部写入的终端号。
   * @param[in] icdev 设备标识符。
   * @param[out] number 返回的终端号字符串，请至少分配33个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetDeviceTerminalNumber(HANDLE icdev, char *number);

  /**
   * @brief  读取设备自定义信息。
   * @par    说明：
   * 读取设备自定义信息。
   * @param[in] icdev 设备标识符。
   * @param[out] info 返回的信息，固定为8个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ReadDeviceCustomInfo(HANDLE icdev, unsigned char *info);

  /**
   * @brief  写入设备自定义信息。
   * @par    说明：
   * 写入设备自定义信息。
   * @param[in] icdev 设备标识符。
   * @param[in] info 传入信息，固定为8个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_WriteDeviceCustomInfo(HANDLE icdev, const unsigned char *info);

  /**
   * @brief  读取设备型号名称。
   * @par    说明：
   * 读取设备型号名称。
   * @param[in] icdev 设备标识符。
   * @param[out] name 返回的型号名称字符串，请至少分配33个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ReadDeviceTypeName(HANDLE icdev, char *name);

  /**
   * @brief  写入设备型号名称。
   * @par    说明：
   * 写入设备型号名称。
   * @param[in] icdev 设备标识符。
   * @param[in] name 传入型号名称，数据格式为'\0'结尾的字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_WriteDeviceTypeName(HANDLE icdev, const char *name);

  /**
   * @brief  无线模块电源控制。
   * @par    说明：
   * 无线模块电源控制。
   * @param[in] icdev 设备标识符。
   * @param[in] value 开关，0x00表示关闭，0x01表示开启。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_WirelessPowerControl(HANDLE icdev, unsigned char value);

  /**
   * @brief  打开TCP链接。
   * @par    说明：
   * 打开TCP链接。
   * @param[in] icdev 设备标识符。
   * @param[in] type 无线模块类型，0x00表示CDMA，0x01表示GPRS，0x02表示WIFI。
   * @param[in] network_address IP地址或域名，数据格式为'\0'结尾的字符串。
   * @param[in] network_port IP端口号，数据格式为'\0'结尾的字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_WirelessTcpOpen(HANDLE icdev, unsigned char type, const char *network_address, const char *network_port);

  /**
   * @brief  关闭TCP链接。
   * @par    说明：
   * 关闭TCP链接。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_WirelessTcpClose(HANDLE icdev);

  /**
   * @brief  查询网络状态。
   * @par    说明：
   * 查询网络状态。
   * @param[in] icdev 设备标识符。
   * @param[out] value 状态值，固定为1个字节。
   * @n 0x00 - 表示TCP初始状态。
   * @n 0x01 - 表示无线模块无法通讯。
   * @n 0x02 - 表示无线模块电源关闭。
   * @n 0x03 - 表示无线模块电源开启。
   * @n 0x04 - 表示SIM卡异常或者无卡。
   * @n 0x05 - 表示网络信号异常。
   * @n 0x06 - 表示无线模块自检成功。
   * @n 0x07 - 表示TCP空闲状态。
   * @n 0x08 - 表示TCP已经连接成功。
   * @n 0x09 - 表示TCP已经断开连接。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_WirelessGetStatus(HANDLE icdev, unsigned char *value);

  /**
   * @brief  发送数据到网络端。
   * @par    说明：
   * 发送数据到网络端。
   * @param[in] icdev 设备标识符。
   * @param[in] type 无线模块类型，0x00表示CDMA，0x01表示GPRS，0x02表示WIFI。
   * @param[in] data_buffer 发送数据。
   * @param[in] data_len 发送数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_WirelessSend(HANDLE icdev, unsigned char type, const unsigned char *data_buffer, int data_len);

  /**
   * @brief  从网络端接收数据。
   * @par    说明：
   * 从网络端接收数据。
   * @param[in] icdev 设备标识符。
   * @param[in] type 无线模块类型，0x00表示CDMA，0x01表示GPRS，0x02表示WIFI。
   * @param[in] mode 模式，0x00表示接收的数据格式不带格式头符号，0x01表示接收的数据格式带格式头符号，WIFI模块此参数无效。
   * @param[in] time_100ms 设备超时值，单位为100毫秒。
   * @param[out] data_buffer 返回的数据。
   * @param[out] data_len 返回数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_WirelessRecv(HANDLE icdev, unsigned char type, unsigned char mode, unsigned char time_100ms, unsigned char *data_buffer, int *data_len);

  /**
   * @brief  配置WIFI参数。
   * @par    说明：
   * 配置WIFI参数。
   * @param[in] icdev 设备标识符。
   * @param[in] type 无线模块类型，0x00表示CDMA，0x01表示GPRS，0x02表示WIFI。
   * @param[in] ssid WIFI SSID，数据格式为'\0'结尾的字符串。
   * @param[in] password WIFI密码，数据格式为'\0'结尾的字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_WirelessWifiConfig(HANDLE icdev, unsigned char type, const char *ssid, const char *password);

  /**
   * @brief  获取GPS信息。
   * @par    说明：
   * 获取GPS信息。
   * @param[in] icdev 设备标识符。
   * @param[in] time_ms 设备超时值，单位为毫秒。
   * @param[out] data_buffer 返回的数据。
   * @param[out] data_len 返回数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GpsGetInfo(HANDLE icdev, int time_ms, unsigned char *data_buffer, int *data_len);

  /**
   * @brief  设置接触式CPU卡616C配置。
   * @par    说明：
   * 设置接触式CPU卡616C配置。
   * @param[in] icdev 设备标识符。
   * @param[in] value 配置值，0x00表示设备不处理616C，0x01表示设备处理616C。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SetCpuCard616C(HANDLE icdev, unsigned char value);

  /**
   * @brief  获取接触式CPU卡616C配置。
   * @par    说明：
   * 获取接触式CPU卡616C配置。
   * @param[in] icdev 设备标识符。
   * @param[out] value 配置值，0x00表示设备不处理616C，0x01表示设备处理616C。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetCpuCard616C(HANDLE icdev, unsigned char *value);

  /**
   * @brief  获取芯片唯一ID号。
   * @par    说明：
   * 获取芯片唯一ID号。
   * @param[in] icdev 设备标识符。
   * @param[out] number 返回的ID号字符串，请至少分配128个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetChipIdNumber(HANDLE icdev, char *number);

  /**
   * @brief  数码管显示。
   * @par    说明：
   * 数码管显示。
   * @param[in] icdev 设备标识符。
   * @param[in] number 数码管编号（1~12）。
   * @param[in] mode 模式，0x00表示数据正常显示，0x01表示数据闪烁显示，0x02表示数据不闪烁显示，0x03表示数据带小数点显示。
   * @param[in] value 显示数据值。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_LedDisplay(HANDLE icdev, unsigned char number, unsigned char mode, unsigned char value);

  /**
   * @brief  数码管显示设置。
   * @par    说明：
   * 数码管显示设置。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 模式。
   * @n 0x00 - 左移。
   * @n 0x01 - 右移。
   * @n 0x02 - 左循环。
   * @n 0x03 - 右循环。
   * @n 0x04 - 显示清除。
   * @n 0x05 - 显示置位。
   * @n 0x06 - 显示亮度增加。
   * @n 0x07 - 显示亮度降低。
   * @n 0x08 - 开显示。
   * @n 0x09 - 关显示。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_LedControl(HANDLE icdev, unsigned char mode);

  /**
   * @brief  透传CRC模式配置。
   * @par    说明：
   * 透传CRC模式配置。
   * @param[in] icdev 设备标识符。
   * @param[in] tx_crc TXCRC模式，0x00表示内部不处理TXCRC，0x01表示内部处理TXCRC（只支持ISO14443标准卡）。
   * @param[in] rx_crc RXCRC模式，0x00表示内部不处理RXCRC，0x01表示内部处理RXCRC（只支持ISO14443标准卡）。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_RfCrcModeConfig(HANDLE icdev, unsigned char tx_crc, unsigned char rx_crc);

  /**
   * @brief  获取TCP信息。
   * @par    说明：
   * 获取TCP信息。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类型，0x01表示请求获得模块型号，0x02表示请求获得当前信号强度。
   * @param[out] data_buffer 返回的数据。
   * @param[out] data_len 返回数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_TcpGetInfo(HANDLE icdev, unsigned char type, unsigned char *data_buffer, int *data_len);

  /**
   * @brief  射频天线打开。
   * @par    说明：
   * 射频天线打开。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_RfMarch(HANDLE icdev);

  /**
   * @brief  选择天线。
   * @par    说明：
   * 选择天线。
   * @param[in] icdev 设备标识符。
   * @param[in] number 天线编号，从1开始编号。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SelectAntenna(HANDLE icdev, unsigned char number);

  /**
   * @brief  读网卡MAC地址。
   * @par    说明：
   * 读网卡MAC地址。
   * @param[in] icdev 设备标识符。
   * @param[out] data_buffer 返回的网卡地址。
   * @param[out] data_len 返回网卡地址的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ReadNicMacAddress(HANDLE icdev, unsigned char *data_buffer, int *data_len);

  /**
   * @brief  写网卡MAC地址。
   * @par    说明：
   * 写网卡MAC地址。
   * @param[in] icdev 设备标识符。
   * @param[in] data_buffer 传入网卡地址。
   * @param[in] data_len 传入网卡地址的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_WriteNicMacAddress(HANDLE icdev, const unsigned char *data_buffer, int data_len);

  /**
   * @brief  测试网卡。
   * @par    说明：
   * 测试网卡。
   * @param[in] icdev 设备标识符。
   * @param[in] data_buffer 传入IP地址。
   * @param[in] data_len 传入IP地址的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_TestNic(HANDLE icdev, const unsigned char *data_buffer, int data_len);

  /**
   * @brief  测试USB。
   * @par    说明：
   * 测试USB。
   * @param[in] icdev 设备标识符。
   * @param[in] data_buffer 保留。
   * @param[in] data_len 保留，固定为0。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_TestUsb(HANDLE icdev, const unsigned char *data_buffer, int data_len);

  /**
   * @brief  测试串口。
   * @par    说明：
   * 测试串口。
   * @param[in] icdev 设备标识符。
   * @param[in] number 串口编号（1~3）。
   * @param[in] name 主机串口名称，如："COM1:115200"。
   * @param[in,out] data_buffer 通讯数据。
   * @param[in,out] data_len 通讯数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_TestCom(HANDLE icdev, unsigned char number, const char *name, unsigned char *data_buffer, int *data_len);

  /**
   * @brief  设定临时IP地址。
   * @par    说明：
   * 设定临时IP地址。
   * @param[in] icdev 设备标识符。
   * @param[in] data_buffer 传入IP地址。
   * @param[in] data_len 传入IP地址的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ConfigTempIpAddress(HANDLE icdev, const unsigned char *data_buffer, int data_len);

  /**
   * @brief  磁条卡数据自动上传数据模式获取。
   * @par    说明：
   * 获取磁条卡数据自动上传数据模式。
   * @param[in] icdev 设备标识符。
   * @param[out] mode 模式。
   * @n 0 - 输出数据中如果带字母，字母转化为大写字母。
   * @n 1 - 输出数据是原始数据（默认）。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetMagFormat(HANDLE icdev, int *mode);

  /**
   * @brief  磁条卡参数获取。
   * @par    说明：
   * 获取操作磁条卡的参数，参数仅针对自动输出的模式有效。
   * @param[in] icdev 设备标识符。
   * @param[out] mode 模式，0x00表示不用输出，0x01表示自动输出。
   * @param[out] track1 1轨道参数。
   * @n bit0 - 轨道数据输出设置，0表示不输出，1表示输出。
   * @n bit1 - 轨道数据头设置，0表示无数据头，1表示有数据头。
   * @n bit2 - 轨道数据尾设置，0表示无数据尾，1表示有数据尾。
   * @n bit3 - 轨道数据换行符设置，0表示无换行符，1表示有换行符。
   * @n bit4 - 轨道数据等号与其后数据设置，0表示不输出，1表示输出。
   * @n bit5~bit7 - 保留。
   * @param[out] track2 2轨道参数。
   * @n bit0 - 轨道数据输出设置，0表示不输出，1表示输出。
   * @n bit1 - 轨道数据头设置，0表示无数据头，1表示有数据头。
   * @n bit2 - 轨道数据尾设置，0表示无数据尾，1表示有数据尾。
   * @n bit3 - 轨道数据换行符设置，0表示无换行符，1表示有换行符。
   * @n bit4 - 轨道数据等号与其后数据设置，0表示不输出，1表示输出。
   * @n bit5~bit7 - 保留。
   * @param[out] track3 3轨道参数。
   * @n bit0 - 轨道数据输出设置，0表示不输出，1表示输出。
   * @n bit1 - 轨道数据头设置，0表示无数据头，1表示有数据头。
   * @n bit2 - 轨道数据尾设置，0表示无数据尾，1表示有数据尾。
   * @n bit3 - 轨道数据换行符设置，0表示无换行符，1表示有换行符。
   * @n bit4 - 轨道数据等号与其后数据设置，0表示不输出，1表示输出。
   * @n bit5~bit7 - 保留。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetMag(HANDLE icdev, unsigned char *mode, unsigned char *track1, unsigned char *track2, unsigned char *track3);

  /**
   * @brief  获取串口设备通讯波特率。
   * @par    说明：
   * 获取串口设备通讯波特率。
   * @param[in] icdev 设备标识符。
   * @param[out] baud 波特率。
   * @n 0x00 - 115200。
   * @n 0x01 - 57600。
   * @n 0x02 - 38400。
   * @n 0x03 - 19200。
   * @n 0x04 - 9600。
   * @n 0x05 - 4800。
   * @n 0x06 - 2400。
   * @n 0x07 - 1200。
   * @n 0x08 - 460800。
   * @n 0x09 - 128000。
   * @n 0x0A - 230400。
   * @n 0x0B - 256000。
   * @n 0x0C - 921600。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetDeviceBaud(HANDLE icdev, unsigned char *baud);

  /**
   * @brief  获取USB模式。
   * @par    说明：
   * 获取USB通讯模式。
   * @param[in] icdev 设备标识符。
   * @param[out] ucmode 模式。
   * @n 0x00 - 表示中断发送和中断接收，此时VID=257B，PID=3010。
   * @n 0x01 - 表示控制发送和控制接收，此时VID=257B，PID=3011。
   * @n 0x02 - 表示控制发送和中断接收，此时VID=0471，PID=7002。
   * @n 0x03 - 表示控制发送和中断接收，此时VID=0471，PID=7003。
   * @n 0x04 - 表示控制发送和中断接收，此时VID=0471，PID=7003，支持磁条卡自动上传功能。
   * @n 0x05 - 虚拟串口。
   * @n 0x06 - 虚拟串口，带复合HID，此时HID的VID=0471，PID=7003。
   * @n 0x07 - 表示控制发送和中断接收，此时VID=0471，PID=7002，支持磁条卡自动上传功能。
   * @n 0x08 - 表示控制发送和中断接收，此时VID=257B，PID=3031。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetUsbMode(HANDLE icdev, unsigned char *ucmode);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * 对当前卡座CPU卡进行指令交互操作，注意此接口已封装卡协议部分，内部处理了SW1为0x61和0x6C的情况。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_cpuapduresponInt(HANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * ::dc_cpuapduresponInt 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_cpuapduresponInt_hex(HANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer);

  /**
   * @brief  开始扫描二维码。
   * @par    说明：
   * 开始扫描二维码。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 保留，固定为0x00。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_Scan2DBarcodeStart(HANDLE icdev, unsigned char mode);

  /**
   * @brief  获取二维码数据。
   * @par    说明：
   * 获取二维码数据。
   * @param[in] icdev 设备标识符。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_Scan2DBarcodeGetData(HANDLE icdev, int *rlen, unsigned char *rdata);

  /**
   * @brief  退出扫描二维码。
   * @par    说明：
   * 退出扫描二维码。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_Scan2DBarcodeExit(HANDLE icdev);

  /**
   * @brief  语音播放及显示。
   * @par    说明：
   * 播放设备内置语音以及是否需要显示。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 语音段号（0~8）。
   * @n 0 - 请输入密码。
   * @n 1 - 请输入原密码。
   * @n 2 - 请再输入一次。
   * @n 3 - 请输入新密码。
   * @n 4 - 请确认新密码。
   * @n 5 - 密码修改成功。
   * @n 6 - 请插卡。
   * @n 7 - 请刷卡。
   * @n 8 - 密码错误。
   * @param[in] type 显示类型，0表示显示语音播放内容，1表示不显示语音播放内容。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_VoicePlayAndDisplay(HANDLE icdev, unsigned char mode, unsigned char type);

  /**
   * @brief  清除设备配置。
   * @par    说明：
   * 清除设备配置。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ClearDeviceConfig(HANDLE icdev);

  /**
   * @brief  接触式CPU卡PPS配置设置。
   * @par    说明：
   * 接触式CPU卡PPS配置设置。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 模式，0表示底层不支持PPS，1表示底层支持PPS。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SetCpuCardPps(HANDLE icdev, unsigned char mode);

  /**
   * @brief  接触式CPU卡PPS配置获取。
   * @par    说明：
   * 接触式CPU卡PPS配置获取。
   * @param[in] icdev 设备标识符。
   * @param[out] mode 模式，0表示底层不支持PPS，1表示底层支持PPS。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetCpuCardPps(HANDLE icdev, unsigned char *mode);

  /**
   * @brief  二维码自动输出配置设置。
   * @par    说明：
   * 二维码自动输出配置设置。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 模式，0表示不输出，1表示输出。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_Set2DBarcodeConfig(HANDLE icdev, unsigned char mode);

  /**
   * @brief  二维码自动输出配置获取。
   * @par    说明：
   * 二维码自动输出配置获取。
   * @param[in] icdev 设备标识符。
   * @param[out] mode 模式，0表示不输出，1表示输出。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_Get2DBarcodeConfig(HANDLE icdev, unsigned char *mode);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_checkcopy_m1(HANDLE icdev, unsigned char mode);

  /**
   * @brief  刷新返回主界面。
   * @par    说明：
   * 刷新返回主界面。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_RefreshAndDisplayMainUI(HANDLE icdev);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_GlyProCommand(HANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer, unsigned char timeout);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_GlyProCommand_hex(HANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer, unsigned char timeout);

  /**
   * @brief  读取SAM_A模块序列号。
   * @par    说明：
   * 读取SAM_A模块序列号。
   * @param[in] icdev 设备标识符。
   * @param[out] number 返回的序列号字符串，请至少分配64个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SamAReadSerialNumber(HANDLE icdev, char *number);

  /**
   * @brief  2.4G APDU指令交互。
   * @par    说明：
   * 2.4G APDU指令交互。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_2dot4Apdu(HANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer);

  /**
   * @brief  2.4G APDU指令交互。
   * @par    说明：
   * ::dc_2dot4Apdu 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_2dot4Apdu_hex(HANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer);

  /**
   * @brief  显示图像。
   * @par    说明：
   * 在屏幕上显示图像，这里的图像指的是前景图像。
   * @param[in] icdev 设备标识符。
   * @param[in] x 图像显示的起始X坐标。
   * @param[in] y 图像显示的起始Y坐标。
   * @param[in] w 图像显示的宽度。
   * @param[in] h 图像显示的高度。
   * @param[in] time_s 显示时间，单位为秒，0表示一直显示。
   * @param[in] image_data 图像数据。格式为：【4个字节图像信息长度（Big-Endian）】【图像信息】。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ScreenDisplayImage(HANDLE icdev, int x, int y, int w, int h, int time_s, const unsigned char *image_data);

  /**
   * @brief  清除显示。
   * @par    说明：
   * 清除屏幕上的显示，这里清除的显示为前景。
   * @param[in] icdev 设备标识符。
   * @param[in] x 清除显示的起始X坐标。
   * @param[in] y 清除显示的起始Y坐标。
   * @param[in] w 清除显示的宽度。
   * @param[in] h 清除显示的高度。
   * @param[in] time_s 延时清除时间，单位为秒，0表示立即清除。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ScreenClearDisplay(HANDLE icdev, int x, int y, int w, int h, int time_s);

  /**
   * @brief  装载背景图像。
   * @par    说明：
   * 装载背景图像到设备内部。
   * @param[in] icdev 设备标识符。
   * @param[in] number 图像序号，从1开始编号。
   * @param[in] image_data 图像数据。格式为：【4个字节图像信息长度（Big-Endian）】【图像信息】。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ScreenLoadBgImage(HANDLE icdev, int number, const unsigned char *image_data);

  /**
   * @brief  显示背景图像。
   * @par    说明：
   * 显示设备内部装载的背景图像。
   * @param[in] icdev 设备标识符。
   * @param[in] number 图像序号，从1开始编号。
   * @param[in] x 图像显示的起始X坐标。
   * @param[in] y 图像显示的起始Y坐标。
   * @param[in] w 图像显示的宽度。
   * @param[in] h 图像显示的高度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ScreenDisplayBgImage(HANDLE icdev, int number, int x, int y, int w, int h);

  /**
   * @brief  获取显示配置。
   * @par    说明：
   * 获取显示配置。
   * @param[in] icdev 设备标识符。
   * @param[in] tag 标签。
   * @n 0x01 - 熄屏时间，配置信息长度固定为4个字节，单位为秒（Big-Endian）。如：config_data = {0x04,0x00,0x00,0x00,0x10}表示熄屏时间为16秒。
   * @n 0x02 - 显示分辨率，配置信息长度固定为4个字节，前2个字节为水平分辨率（Big-Endian），后2个字节为垂直分辨率（Big-Endian）。如：config_data = {0x04,0x02,0x80,0x01,0xE0}表示分辨率为640x480。
   * @param[out] config_data 配置数据。格式为：【1字节配置信息长度】【配置信息】。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ScreenDisplayGetConfig(HANDLE icdev, unsigned char tag, unsigned char *config_data);

  /**
   * @brief  设置显示配置。
   * @par    说明：
   * 设置显示配置。
   * @param[in] icdev 设备标识符。
   * @param[in] tag 标签。
   * @n 0x01 - 熄屏时间，配置信息长度固定为4个字节，单位为秒（Big-Endian）。如：config_data = {0x04,0x00,0x00,0x00,0x10}表示熄屏时间为16秒。
   * @n 0x02 - 显示分辨率，配置信息长度固定为4个字节，前2个字节为水平分辨率（Big-Endian），后2个字节为垂直分辨率（Big-Endian）。如：config_data = {0x04,0x02,0x80,0x01,0xE0}表示分辨率为640x480。
   * @param[in] config_data 配置数据。格式为：【1字节配置信息长度】【配置信息】。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ScreenDisplaySetConfig(HANDLE icdev, unsigned char tag, const unsigned char *config_data);

  /**
   * @brief  显示文字。
   * @par    说明：
   * 在屏幕上显示文字，注意文字也属于前景。
   * @param[in] icdev 设备标识符。
   * @param[in] font_size 字体大小。
   * @param[in] fg_rgb 文字前景色，3个字节RGB值，第一个字节为R，第二个字节为G，第三个字节为B。
   * @param[in] bg_rgb 文字背景色，3个字节RGB值，第一个字节为R，第二个字节为G，第三个字节为B。
   * @param[in] x 文字显示的起始X坐标。
   * @param[in] y 文字显示的起始Y坐标。
   * @param[in] time_s 显示时间，单位为秒，0表示一直显示。
   * @param[in] line_flag 换行标志，0表示不自动换行，1表示自动换行。
   * @param[in] voice_flag 语音标志，0表示不播放语音，1表示播放语音。
   * @param[in] text 文本字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ScreenDisplayText(HANDLE icdev, unsigned char font_size, const unsigned char *fg_rgb, const unsigned char *bg_rgb, int x, int y, int time_s, unsigned char line_flag, unsigned char voice_flag, const char *text);

  /**
   * @brief  填充背景。
   * @par    说明：
   * 使用指定颜色填充背景。
   * @param[in] icdev 设备标识符。
   * @param[in] rgb 颜色，3个字节RGB值，第一个字节为R，第二个字节为G，第三个字节为B。
   * @param[in] x 起始X坐标。
   * @param[in] y 起始Y坐标。
   * @param[in] w 宽度。
   * @param[in] h 高度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ScreenDisplayBgImageByColor(HANDLE icdev, const unsigned char *rgb, int x, int y, int w, int h);

  /**
   * @brief  显示二维码。
   * @par    说明：
   * 显示二维码。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类别，0x01表示正方形二维码。
   * @param[in] x 起始X坐标。
   * @param[in] y 起始Y坐标。
   * @param[in] w 宽度。
   * @param[in] h 高度。
   * @param[in] content 数据内容，一个或多个TLV格式，T和L分别占用4个字节（Big-Endian）。
   * @n T为1表示值为ASCII文本字符。
   * @param[in] content_len 数据内容的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ScreenDisplay2DBarcode(HANDLE icdev, unsigned char type, int x, int y, int w, int h, const unsigned char *content, int content_len);

  /**
   * @brief  NTAG标签交互。
   * @par    说明：
   * NTAG标签交互。
   * @param[in] icdev 设备标识符。
   * @param[in] type 保留，固定为0x00。
   * @param[in] mode 模式。
   * @param[in] slen 发送数据的长度。
   * @param[in] sdata 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_NtagCommand(HANDLE icdev, unsigned char type, unsigned char mode, unsigned int slen, const unsigned char *sdata, unsigned int *rlen, unsigned char *rdata);

  /**
   * @brief  设置接触式CPU卡电压。
   * @par    说明：
   * 设置接触式CPU卡电压。
   * @param[in] icdev 设备标识符。
   * @param[in] voltage 电压值，0x00表示5V，0x01表示3.3V，0x02表示1.8V。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SetCpuVoltage(HANDLE icdev, unsigned char voltage);

  /**
   * @brief  LCD显示文字。
   * @par    说明：
   * 在设备LCD显示屏上显示文字。
   * @param[in] icdev 设备标识符。
   * @param[in] line 行号。
   * @param[in] offset 偏移。
   * @param[in] mode 模式，0表示正色，1表示反色。
   * @param[in] charset 字符集。
   * @n 0 - ASCII。
   * @n 1 - GB2312。
   * @n 2 - GBK。
   * @n 3 - GB18030。
   * @n 4 - ANSI。
   * @n 5 - UNICODE（Little-Endian）。
   * @n 6 - UNICODE（Big-Endian）。
   * @param[in] text 文本字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_LcdDisplayText(HANDLE icdev, unsigned char line, unsigned char offset, unsigned char mode, unsigned char charset, const unsigned char *text);

  /**
   * @brief  LCD显示文字。
   * @par    说明：
   * 通过指定字模点阵数据，在设备LCD显示屏上显示文字。
   * @param[in] icdev 设备标识符。
   * @param[in] line 行号。
   * @param[in] offset 偏移。
   * @param[in] mode 模式，0表示正色，1表示反色。
   * @param[in] charset 字符集。
   * @n 0 - ASCII。
   * @n 1 - GB2312。
   * @n 2 - GBK。
   * @n 3 - GB18030。
   * @n 4 - ANSI。
   * @n 5 - UNICODE（Little-Endian）。
   * @n 6 - UNICODE（Big-Endian）。
   * @param[in] text 文本字符串。
   * @param[in] data 字模点阵数据。
   * @param[in] data_len 字模点阵数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_LcdDisplayTextFromData(HANDLE icdev, unsigned char line, unsigned char offset, unsigned char mode, unsigned char charset, const unsigned char *text, const unsigned char *data, int data_len);

  /**
   * @brief  装载文件数据。
   * @par    说明：
   * 装载文件数据到设备中。
   * @param[in] icdev 设备标识符。
   * @param[in] file_type 文件类型。
   * @param[in] file_size 文件大小。
   * @param[in] flag 传输标志。
   * @n 0 - 准备传输。
   * @n 1~0xFFFFFFFE - 非结尾传输。
   * @n 0xFFFFFFFF - 结尾传输。
   * @param[in] data 数据，当 @a flag 不为0时有效。
   * @param[in,out] data_size 数据大小。
   * @n flag为0 - data_size为Out形式的参数，返回设备支持的最大分包长度。
   * @n flag不为0 - data_size为In形式的参数，传入 @a data 的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_DownloadFileData(HANDLE icdev, unsigned int file_type, unsigned int file_size, unsigned int flag, const unsigned char *data, unsigned int *data_size);

  /**
   * @brief  检测1608卡。
   * @par    说明：
   * 检测是否存在1608卡。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  API_ATTRIBUTE short USER_API dc_check_1608(HANDLE icdev);

  /**
   * @brief  1608卡下电。
   * @par    说明：
   * 对1608卡进行下电操作。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_down_1608(HANDLE icdev);

  /**
   * @brief  读1608卡。
   * @par    说明：
   * 读取1608卡的数据。
   * @param[in] icdev 设备标识符。
   * @param[in] zone 数据区。
   * @n 0~7 - 表示用户区。
   * @n 8 - 表示配置区。
   * @param[in] offset 偏移。
   * @param[in] length 读取长度。
   * @param[out] readdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_read_1608(HANDLE icdev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *readdata);

  /**
   * @brief  读1608卡。
   * @par    说明：
   * ::dc_read_1608 的HEX形式接口，参数 @a readdata 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_read_1608_hex(HANDLE icdev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *readdata);

  /**
   * @brief  写1608卡。
   * @par    说明：
   * 写入数据到1608卡中。
   * @param[in] icdev 设备标识符。
   * @param[in] zone 数据区。
   * @n 0~7 - 表示用户区。
   * @n 8 - 表示配置区。
   * @param[in] offset 偏移。
   * @param[in] length 写入长度。
   * @param[in] writedata 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_write_1608(HANDLE icdev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *writedata);

  /**
   * @brief  写1608卡。
   * @par    说明：
   * ::dc_write_1608 的HEX形式接口，参数 @a writedata 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_write_1608_hex(HANDLE icdev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *writedata);

  /**
   * @brief  验证1608卡密码。
   * @par    说明：
   * 使用传入的密码来验证1608卡密码。
   * @param[in] icdev 设备标识符。
   * @param[in] zone 数据区。
   * @n 0~7 - 表示用户区。
   * @n 8 - 表示配置区。
   * @param[in] type 类型，0表示验证写密钥，1表示验证读密钥。
   * @param[in] password 密码，固定为3个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_checkpass_1608(HANDLE icdev, unsigned char zone, unsigned char type, unsigned char *password);

  /**
   * @brief  验证1608卡密码。
   * @par    说明：
   * ::dc_checkpass_1608 的HEX形式接口，参数 @a password 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_checkpass_1608_hex(HANDLE icdev, unsigned char zone, unsigned char type, unsigned char *password);

  /**
   * @brief  初始化1608卡认证。
   * @par    说明：
   * 使用传入的数据来初始化1608卡认证。
   * @param[in] icdev 设备标识符。
   * @param[in] databuffer 数据，固定为8个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_init_auth_1608(HANDLE icdev, unsigned char *databuffer);

  /**
   * @brief  初始化1608卡认证。
   * @par    说明：
   * ::dc_init_auth_1608 的HEX形式接口，参数 @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_init_auth_1608_hex(HANDLE icdev, unsigned char *databuffer);

  /**
   * @brief  核对1608卡认证。
   * @par    说明：
   * 使用传入的数据来核对1608卡认证。
   * @param[in] icdev 设备标识符。
   * @param[in] databuffer 数据，固定为8个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_check_auth_1608(HANDLE icdev, unsigned char *databuffer);

  /**
   * @brief  核对1608卡认证。
   * @par    说明：
   * ::dc_check_auth_1608 的HEX形式接口，参数 @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_check_auth_1608_hex(HANDLE icdev, unsigned char *databuffer);

  /**
   * @brief  写1608卡熔丝。
   * @par    说明：
   * 写1608卡熔丝。
   * @param[in] icdev 设备标识符。
   * @param[in] value 保留，固定为0x00。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_write_fuse_1608(HANDLE icdev, unsigned char value);

  /**
   * @brief  读1608卡熔丝。
   * @par    说明：
   * 读1608卡熔丝。
   * @param[in] icdev 设备标识符。
   * @param[out] value 状态值。
   * @n bit0 - FAB，Atmel公司发行熔丝，0表示已熔丝，1表示未熔丝。
   * @n bit1 - CMA，卡发行商发行熔丝，0表示已熔丝，1表示未熔丝。
   * @n bit2 - PER，个人化发行熔丝，0表示已熔丝，1表示未熔丝。
   * @n bit3~bit7 - 保留。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_read_fuse_1608(HANDLE icdev, unsigned char *value);

  /**
   * @brief  检测153卡。
   * @par    说明：
   * 检测是否存在153卡。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败或不存在，==0表示存在。
   */
  API_ATTRIBUTE short USER_API dc_check_153(HANDLE icdev);

  /**
   * @brief  153卡下电。
   * @par    说明：
   * 对153卡进行下电操作。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_down_153(HANDLE icdev);

  /**
   * @brief  读153卡。
   * @par    说明：
   * 读取153卡的数据。
   * @param[in] icdev 设备标识符。
   * @param[in] zone 数据区。
   * @n 0~2 - 表示用户区。
   * @n 3 - 表示配置区。
   * @param[in] offset 偏移。
   * @param[in] length 读取长度。
   * @param[out] readdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_read_153(HANDLE icdev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *readdata);

  /**
   * @brief  读153卡。
   * @par    说明：
   * ::dc_read_153 的HEX形式接口，参数 @a readdata 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_read_153_hex(HANDLE icdev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *readdata);

  /**
   * @brief  写153卡。
   * @par    说明：
   * 写入数据到153卡中。
   * @param[in] icdev 设备标识符。
   * @param[in] zone 数据区。
   * @n 0~2 - 表示用户区。
   * @n 3 - 表示配置区。
   * @param[in] offset 偏移。
   * @param[in] length 写入长度。
   * @param[in] writedata 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_write_153(HANDLE icdev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *writedata);

  /**
   * @brief  写153卡。
   * @par    说明：
   * ::dc_write_153 的HEX形式接口，参数 @a writedata 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_write_153_hex(HANDLE icdev, unsigned char zone, unsigned int offset, unsigned int length, unsigned char *writedata);

  /**
   * @brief  验证153卡密码。
   * @par    说明：
   * 使用传入的密码来验证153卡密码。
   * @param[in] icdev 设备标识符。
   * @param[in] zone 数据区。
   * @n 0~2 - 表示用户区。
   * @n 3 - 表示配置区。
   * @param[in] type 类型，0表示验证写密钥，1表示验证读密钥。
   * @param[in] password 密码，固定为3个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_checkpass_153(HANDLE icdev, unsigned char zone, unsigned char type, unsigned char *password);

  /**
   * @brief  验证153卡密码。
   * @par    说明：
   * ::dc_checkpass_153 的HEX形式接口，参数 @a password 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_checkpass_153_hex(HANDLE icdev, unsigned char zone, unsigned char type, unsigned char *password);

  /**
   * @brief  初始化153卡认证。
   * @par    说明：
   * 使用传入的数据来初始化153卡认证。
   * @param[in] icdev 设备标识符。
   * @param[in] databuffer 数据，固定为8个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_init_auth_153(HANDLE icdev, unsigned char *databuffer);

  /**
   * @brief  初始化153卡认证。
   * @par    说明：
   * ::dc_init_auth_153 的HEX形式接口，参数 @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_init_auth_153_hex(HANDLE icdev, unsigned char *databuffer);

  /**
   * @brief  核对153卡认证。
   * @par    说明：
   * 使用传入的数据来核对153卡认证。
   * @param[in] icdev 设备标识符。
   * @param[in] databuffer 数据，固定为8个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_check_auth_153(HANDLE icdev, unsigned char *databuffer);

  /**
   * @brief  核对153卡认证。
   * @par    说明：
   * ::dc_check_auth_153 的HEX形式接口，参数 @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_check_auth_153_hex(HANDLE icdev, unsigned char *databuffer);

  /**
   * @brief  写153卡熔丝。
   * @par    说明：
   * 写153卡熔丝。
   * @param[in] icdev 设备标识符。
   * @param[in] value 保留，固定为0x00。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_write_fuse_153(HANDLE icdev, unsigned char value);

  /**
   * @brief  读153卡熔丝。
   * @par    说明：
   * 读153卡熔丝。
   * @param[in] icdev 设备标识符。
   * @param[out] value 状态值。
   * @n bit0 - FAB，Atmel公司发行熔丝，0表示已熔丝，1表示未熔丝。
   * @n bit1 - CMA，卡发行商发行熔丝，0表示已熔丝，1表示未熔丝。
   * @n bit2 - PER，个人化发行熔丝，0表示已熔丝，1表示未熔丝。
   * @n bit3~bit7 - 保留。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_read_fuse_153(HANDLE icdev, unsigned char *value);

  /**
   * @brief  多天线启动或停止。
   * @par    说明：
   * 多天线启动或停止。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 标志，0表示停止，1表示启动。
   * @param[in,out] mask 掩码值，32bit分别表示32路天线，相应bit位为1表示天线有效，传入为期待值，返回为实际值。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_MultiAntennaStartOrStop(HANDLE icdev, unsigned char flag, unsigned int *mask);

  /**
   * @brief  多天线获取卡状态。
   * @par    说明：
   * 多天线获取卡状态。
   * @param[in] icdev 设备标识符。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据。
   * @n 0x00 - 无卡。
   * @n 0x8A - 有卡，Type A CPU卡。
   * @n 0x8B - 有卡，Type B CPU卡。
   * @n 0x80 - 有卡，未知卡型。
   * @n 0x81 - 有卡，M1卡。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_MultiAntennaGetCardStatus(HANDLE icdev, int *rlen, unsigned char *rdata);

  /**
   * @brief  设置reader与密码键盘串口波特率。
   * @par    说明：
   * 设置reader与密码键盘串口波特率。
   * @param[in] icdev 设备标识符。
   * @param[in] value 波特率。
   * @n 0x00 - 2400。
   * @n 0x01 - 4800。
   * @n 0x02 - 9600。
   * @n 0x03 - 14400。
   * @n 0x04 - 38400。
   * @n 0x05 - 57600。
   * @n 0x06 - 115200。
   * @n 0x07 - 256000。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SetPinpadWithReaderBaudrate(HANDLE icdev, unsigned char value);

  /**
   * @brief  查询reader与密码键盘串口波特率。
   * @par    说明：
   * 查询reader与密码键盘串口波特率。
   * @param[in] icdev 设备标识符。
   * @param[out] value 波特率。
   * @n 0x00 - 2400。
   * @n 0x01 - 4800。
   * @n 0x02 - 9600。
   * @n 0x03 - 14400。
   * @n 0x04 - 38400。
   * @n 0x05 - 57600。
   * @n 0x06 - 115200。
   * @n 0x07 - 256000。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_CheckPinpadWithReaderBaudrate(HANDLE icdev, unsigned char *value);

  /**
   * @brief  读身份证。
   * @par    说明：
   * 读取身份证的原始信息数据，此接口使用硬件加速，需固件支持。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 模式，为0x00表示定制卡（内部使用），为0x01表示模拟身份证卡，为0x02表示物理身份证卡。
   * @param[in] type 类型，为0x00表示内部使用，否则值为下面列出的一种或多种之和。
   * @n 0x01 - 读取文字信息。
   * @n 0x02 - 读取相片信息。
   * @n 0x04 - 读取指纹信息。
   * @n 0x08 - 读取追加住址信息。
   * @param[out] text_len 返回文字信息的长度。
   * @param[out] text 返回的文字信息，请至少分配256个字节。
   * @param[out] photo_len 返回相片信息的长度。
   * @param[out] photo 返回的相片信息，请至少分配1024个字节。
   * @param[out] fingerprint_len 返回指纹信息的长度。
   * @param[out] fingerprint 返回的指纹信息，请至少分配1024个字节。
   * @param[out] extra_len 返回追加住址信息的长度。
   * @param[out] extra 返回的追加住址信息，请至少分配70个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_IdCardReadCardInfo(HANDLE icdev, unsigned char mode, unsigned char type, int *text_len, unsigned char *text, int *photo_len, unsigned char *photo, int *fingerprint_len, unsigned char *fingerprint, int *extra_len, unsigned char *extra);

  /**
   * @brief  液晶显示。
   * @par    说明：
   * 显示指定字符串到液晶屏幕上。
   * @param[in] icdev 设备标识符。
   * @param[in] line 行号。
   * @param[in] offset 偏移。
   * @param[in] mode 模式，为0x00表示正显，为0x01表示反显。
   * @param[in] data 要显示的字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_LcdDisplay(HANDLE icdev, unsigned char line, unsigned char offset, unsigned char mode, const char *data);

  /**
   * @brief  读设备定制序列号。
   * @par    说明：
   * 读设备定制序列号。
   * @param[in] icdev 设备标识符。
   * @param[in] number 编号，从0开始编号。
   * @param[out] data 序列号字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ReadDeviceCustomSerialNumber(HANDLE icdev, unsigned char number, char *data);

  /**
   * @brief  写设备定制序列号。
   * @par    说明：
   * 写设备定制序列号。
   * @param[in] icdev 设备标识符。
   * @param[in] number 编号，从0开始编号。
   * @param[in] data 序列号字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_WriteDeviceCustomSerialNumber(HANDLE icdev, unsigned char number, const char *data);

  /**
   * @brief  二维码自动上传模式开启/关闭（下电不保留状态）。
   * @par    说明：
   * 二维码自动上传模式开启/关闭（下电不保留状态）。
   * @param[in] icdev 设备标识符。
   * @param[in] value 配置值，0x00表示关闭，0x01表示开启。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_2DBarcodeAutoModeOnOff(HANDLE icdev, unsigned char value);

  /**
   * @brief  获取二维码属性。
   * @par    说明：
   * 获取二维码属性。
   * @param[in] icdev 设备标识符。
   * @param[out] data_len 返回属性的长度。
   * @param[out] data 返回的属性，TLV格式，T和L固定为1个字节。
   * @n tag为0x01时，length为0x01，value为二维码类型。
   * @n value为0表示未解到码。
   * @n value为1表示CODE128码。
   * @n value为2表示CODE39码。
   * @n value为3表示CODE93码。
   * @n value为4表示EAN 13码。
   * @n value为5表示EAN 8码。
   * @n value为6表示UPC_A码。
   * @n value为7表示UPC_E0码。
   * @n value为8表示UPC_E1码。
   * @n value为9表示ISBN13码。
   * @n value为10表示ITF25码。
   * @n value为11表示Matrix 2 of 5码。
   * @n value为12表示Industrial 25码。
   * @n value为13表示Code11码。
   * @n value为14表示Codabar码。
   * @n value为15表示RSS-14码。
   * @n value为16表示RSS Limited码。
   * @n value为17表示RSS Expanded码。
   * @n value为18表示MSI Plessey码。
   * @n value为19表示Standard 25码。
   * @n value为20表示Plessey码。
   * @n value为21表示China Post 25码。
   * @n value为22表示Code16K码。
   * @n value为23表示GS1 Composite码。
   * @n value为24表示QRCode码。
   * @n value为25表示PDF417码。
   * @n value为26表示DataMatrix码。
   * @n value为27表示Han Xin码。
   * @n value为28表示Micro PDF417码。
   * @n value为29表示Micro QRCode码。
   * @n value为30表示MaxiCode码。
   * @n value为31表示Aztec码。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_2DBarcodeGetAttributes(HANDLE icdev, unsigned int *data_len, unsigned char *data);

  /**
   * @brief  读防拆功能区。
   * @par    说明：
   * 读取防拆功能区中的数据。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移地址。
   * @param[in] length 读取长度。
   * @param[out] data_buffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ReadTamperDeviceArea(HANDLE icdev, int offset, int length, unsigned char *data_buffer);

  /**
   * @brief  写防拆功能区。
   * @par    说明：
   * 写入数据到防拆功能区中，可以用作数据保存等。
   * @param[in] icdev 设备标识符。
   * @param[in] offset 偏移地址。
   * @param[in] length 写入长度。
   * @param[in] data_buffer 传入数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_WriteTamperDeviceArea(HANDLE icdev, int offset, int length, const unsigned char *data_buffer);

  /**
   * @brief  通用显示获取屏幕属性。
   * @par    说明：
   * 通用显示获取屏幕相关属性。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类别，0x01表示获取屏幕支持的分辨率和颜色位。
   * @param[out] attribute_len 返回属性数据的长度。
   * @param[out] attribute 返回的属性数据。
   * @n type为0x01时，返回【1个字节支持分辨率和颜色位的条目数量】【分辨率和颜色位1】【分辨率和颜色位2】...。每个分辨率和颜色位条目固定为9个字节，其中位置0~3字节（Big-Endian）表示屏幕宽度（像素），位置4~7字节（Big-Endian）表示屏幕高度（像素），位置8字节表示屏幕单个像素所占用位数。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_DisplayGetScreenAttribute(HANDLE icdev, unsigned char type, int *attribute_len, unsigned char *attribute);

  /**
   * @brief  通用显示启动屏幕绘制。
   * @par    说明：
   * 通用显示启动屏幕绘制，此接口用于准备绘制屏幕的资源。
   * @param[in] icdev 设备标识符。
   * @param[in] width 分辨率宽度。
   * @param[in] height 分辨率高度。
   * @param[in] bpp 颜色位。
   * @n type为0x01时，返回【1个字节支持分辨率和颜色位的条目数量】【分辨率和颜色位1】【分辨率和颜色位2】...。每个分辨率和颜色位条目固定为9个字节，其中位置0~3字节（Big-Endian）表示屏幕宽度（像素），位置4~7字节（Big-Endian）表示屏幕高度（像素），位置8字节表示屏幕单个像素所占用位数。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_DisplayStartScreenDraw(HANDLE icdev, unsigned int width, unsigned int height, unsigned char bpp);

  /**
   * @brief  通用显示绘制屏幕。
   * @par    说明：
   * 通用显示绘制屏幕。
   * @param[in] icdev 设备标识符。
   * @param[in] x 起始X坐标。
   * @param[in] y 起始Y坐标。
   * @param[in] w 宽度。
   * @param[in] h 高度。
   * @param[in] pixel_len 像素数据的长度。
   * @param[in] pixel 像素数据。
   * @param[in] flag 标志，0x00表示不刷新显示（内部使用显示缓存），0x01表示立即刷新显示。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_DisplayDrawScreen(HANDLE icdev, unsigned int x, unsigned int y, unsigned int w, unsigned int h, int pixel_len, const unsigned char *pixel, unsigned char flag);

  /**
   * @brief  通用显示退出绘制并且刷新屏幕。
   * @par    说明：
   * 通用显示退出绘制并且刷新屏幕。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_DisplayExitDrawAndflushScreen(HANDLE icdev);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_YiHuaCryptoGetRandom(HANDLE icdev, unsigned char type, unsigned char length, unsigned char *random_data);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_YiHuaCryptoExternalAuthenticate(HANDLE icdev, unsigned char type, unsigned char length, const unsigned char *crypto_random_data);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_YiHuaCryptoPublicKeyImport(HANDLE icdev, unsigned char type, unsigned char length, const unsigned char *public_key_data);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_YiHuaCryptoSessionKeyNegotiationInit(HANDLE icdev, unsigned char type, unsigned char *cr1_len, unsigned char *cr1_data, unsigned char *cr2_len, unsigned char *cr2_data, unsigned char *cdpk_len, unsigned char *cdpk_data);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_YiHuaCryptoSessionKeyGen(HANDLE icdev, unsigned char type, unsigned char length, const unsigned char *crypto_random_data);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_YiHuaCryptoExternalAuthenticateKeyUpdate(HANDLE icdev, unsigned char type, unsigned char length, const unsigned char *crypto_data);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_YiHuaCryptoReadMag(HANDLE icdev, unsigned char type, unsigned char *t1_len, unsigned char *t1_data, unsigned char *t2_len, unsigned char *t2_data, unsigned char *t3_len, unsigned char *t3_data);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_YiHuaCryptoIcApdu(HANDLE icdev, unsigned char type, int slen, const unsigned char *sdata, int *rlen, unsigned char *rdata);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_YiHuaCryptoRfApdu(HANDLE icdev, unsigned char type, int slen, const unsigned char *sdata, int *rlen, unsigned char *rdata);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_YiHuaExportSessionKey(HANDLE icdev, unsigned char type, int *sk_len, unsigned char *sk_data);

  /**
   * @brief  15693卡透传指令。
   * @par    说明：
   * 15693卡透传指令。
   * @param[in] icdev 设备标识符。
   * @param[in] time_ms 设备超时值，单位为毫秒。
   * @param[in] in_data 输入数据。
   * @param[in] in_len 输入数据的长度。
   * @param[out] out_data 输出数据。
   * @param[out] out_len 输出数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_Iso15693Communication(HANDLE icdev, int time_ms, const unsigned char *in_data, int in_len, unsigned char *out_data, int *out_len);

  /**
   * @brief  射频参数配置。
   * @par    说明：
   * 射频参数配置。
   * @param[in] icdev 设备标识符。
   * @param[in,out] flags 类型标志，可为多个以下值按位或在一起。
   * @n 0x00 - 全部不配置。
   * @n 0x01 - 配置rf_gain有效。
   * @n 0x02 - 配置minimum_power有效。
   * @n 0x04 - 配置couple_power有效。
   * @n 0x08 - 配置modulation_depth有效。
   * @n 0x10 - 配置waveform_adjustment有效。
   * @n 0x20 - 配置rf_strength有效。
   * @param[in] rf_gain 射频输入信号放大。
   * @param[in] minimum_power 最小功率。
   * @param[in] couple_power 耦合功率。
   * @param[in] modulation_depth 调制深度。
   * @param[in] waveform_adjustment 波形调整。
   * @param[in] rf_strength 射频输出场强大小。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SetRfAttribute(HANDLE icdev, unsigned short *flags, unsigned char rf_gain, unsigned char minimum_power, unsigned char couple_power, unsigned char modulation_depth, unsigned char waveform_adjustment, unsigned char rf_strength);

  /**
   * @brief  射频参数获取。
   * @par    说明：
   * 射频参数获取。
   * @param[in] icdev 设备标识符。
   * @param[in,out] flags 类型标志，可为多个以下值按位或在一起。
   * @n 0x00 - 全部不配置。
   * @n 0x01 - 配置rf_gain有效。
   * @n 0x02 - 配置minimum_power有效。
   * @n 0x04 - 配置couple_power有效。
   * @n 0x08 - 配置modulation_depth有效。
   * @n 0x10 - 配置waveform_adjustment有效。
   * @n 0x20 - 配置rf_strength有效。
   * @param[out] rf_gain 射频输入信号放大。
   * @param[out] minimum_power 最小功率。
   * @param[out] couple_power 耦合功率。
   * @param[out] modulation_depth 调制深度。
   * @param[out] waveform_adjustment 波形调整。
   * @param[out] rf_strength 射频输出场强大小。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetRfAttribute(HANDLE icdev, unsigned short *flags, unsigned char *rf_gain, unsigned char *minimum_power, unsigned char *couple_power, unsigned char *modulation_depth, unsigned char *waveform_adjustment, unsigned char *rf_strength);

  /**
   * @brief  LCD显示交互。
   * @par    说明：
   * LCD显示交互。
   * @param[in] icdev 设备标识符。
   * @param[in] type 格式类型，0x00表示XML格式，0x01表示TLV格式，0x02表示JPEG格式。
   * @param[in] slen 发送数据的长度。
   * @param[in] sdata 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_LcdDisplayInteraction(HANDLE icdev, unsigned char type, unsigned int slen, const unsigned char *sdata, unsigned int *rlen, unsigned char *rdata);

  /**
   * @brief  LCD显示交互（查询状态）。
   * @par    说明：
   * LCD显示交互（查询状态）。
   * @param[in] icdev 设备标识符。
   * @param[in] type 格式类型，0x00表示XML格式，0x01表示TLV格式，0x02表示JPEG格式。
   * @param[in] id 用于唯一标识一次完整数据的传输。
   * @param[in] size_of_processed_pc_data 表示PC已处理数据的总大小（单位：字节）。
   * @param[out] size_of_processed_device_data 表示DEVICE已处理数据的总大小（单位：字节）。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_LcdDisplayInteractionQueryState(HANDLE icdev, unsigned char type, unsigned int id, unsigned int size_of_processed_pc_data, unsigned int *size_of_processed_device_data);

  /**
   * @brief  LCD显示交互（请求PC->DEVICE数据传输）。
   * @par    说明：
   * LCD显示交互（请求PC->DEVICE数据传输）。
   * @param[in] icdev 设备标识符。
   * @param[in] type 格式类型，0x00表示XML格式，0x01表示TLV格式，0x02表示JPEG格式。
   * @param[in] id 用于唯一标识一次完整数据的传输。
   * @param[in] pc_capacity_size 表示PC每帧中传输数据的最大值（单位：字节）。
   * @param[in] data_size 要传输数据的总大小（单位：字节）。
   * @param[out] device_capacity_size 表示DEVICE每帧中传输数据的最大值（单位：字节）。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_LcdDisplayInteractionPcToDeviceRequest(HANDLE icdev, unsigned char type, unsigned int id, unsigned int pc_capacity_size, unsigned int data_size, unsigned int *device_capacity_size);

  /**
   * @brief  LCD显示交互（进行PC->DEVICE数据传输）。
   * @par    说明：
   * LCD显示交互（进行PC->DEVICE数据传输）。
   * @param[in] icdev 设备标识符。
   * @param[in] type 格式类型，0x00表示XML格式，0x01表示TLV格式，0x02表示JPEG格式。
   * @param[in] id 用于唯一标识一次完整数据的传输。
   * @param[in,out] offset 传入PC要发送数据的偏移量（单位：字节）并且返回DEVICE实际接收数据的偏移量（单位：字节）。
   * @param[in,out] length 传入PC要发送数据的长度（单位：字节）并且返回DEVICE实际接收数据的长度（单位：字节）。
   * @param[in] data 传入PC要发送的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_LcdDisplayInteractionPcToDeviceRun(HANDLE icdev, unsigned char type, unsigned int id, unsigned int *offset, unsigned int *length, const unsigned char *data);

  /**
   * @brief  LCD显示交互（请求DEVICE->PC数据传输）。
   * @par    说明：
   * LCD显示交互（请求DEVICE->PC数据传输）。
   * @param[in] icdev 设备标识符。
   * @param[in] type 格式类型，0x00表示XML格式，0x01表示TLV格式，0x02表示JPEG格式。
   * @param[in] id 用于唯一标识一次完整数据的传输。
   * @param[in] pc_capacity_size 表示PC每帧中传输数据的最大值（单位：字节）。
   * @param[out] device_capacity_size 表示DEVICE每帧中传输数据的最大值（单位：字节）。
   * @param[out] data_size 要传输数据的总大小（单位：字节）。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_LcdDisplayInteractionDeviceToPcRequest(HANDLE icdev, unsigned char type, unsigned int id, unsigned int pc_capacity_size, unsigned int *device_capacity_size, unsigned int *data_size);

  /**
   * @brief  LCD显示交互（进行DEVICE->PC数据传输）。
   * @par    说明：
   * LCD显示交互（进行DEVICE->PC数据传输）。
   * @param[in] icdev 设备标识符。
   * @param[in] type 格式类型，0x00表示XML格式，0x01表示TLV格式，0x02表示JPEG格式。
   * @param[in] id 用于唯一标识一次完整数据的传输。
   * @param[in,out] offset 传入PC要接收数据的偏移量（单位：字节）并且返回DEVICE实际发送数据的偏移量（单位：字节）。
   * @param[in,out] length 传入PC要接收数据的长度（单位：字节）并且返回DEVICE实际发送数据的长度（单位：字节）。
   * @param[out] data 返回DEVICE实际发送的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_LcdDisplayInteractionDeviceToPcRun(HANDLE icdev, unsigned char type, unsigned int id, unsigned int *offset, unsigned int *length, unsigned char *data);

  /**
   * @brief  FeliCa卡复位。
   * @par    说明：
   * FeliCa卡复位。
   * @param[in] icdev 设备标识符。
   * @param[out] uid_len 返回UID的长度。
   * @param[out] uid_data 返回的UID，请至少分配64个字节。
   * @param[out] ats_len 返回复位信息的长度。
   * @param[out] ats_data 返回的复位信息，请至少分配128个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_FeliCaReset(HANDLE icdev, unsigned char *uid_len, unsigned char *uid_data, unsigned char *ats_len, unsigned char *ats_data);

  /**
   * @brief  FeliCa卡复位。
   * @par    说明：
   * ::dc_FeliCaReset 的HEX形式接口，参数 @a uid_data @a ats_data 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_FeliCaReset_hex(HANDLE icdev, unsigned char *uid_len, char *uid_data, unsigned char *ats_len, char *ats_data);

  /**
   * @brief  FeliCa卡指令交互。
   * @par    说明：
   * FeliCa卡指令交互。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sdata 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_FeliCaApdu(HANDLE icdev, unsigned int slen, const unsigned char *sdata, unsigned int *rlen, unsigned char *rdata);

  /**
   * @brief  FeliCa卡指令交互。
   * @par    说明：
   * ::dc_FeliCaApdu 的HEX形式接口，参数 @a sdata @a rdata 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_FeliCaApdu_hex(HANDLE icdev, unsigned int slen, const char *sdata, unsigned int *rlen, char *rdata);

  /**
   * @brief  读取磁条卡原始数据。
   * @par    说明：
   * 读取磁条卡原始数据。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 模式，0表示原始数据，1表示二进制数据流，2表示ISO7811协议解析数据。
   * @param[in] time_ms 设备超时值，单位为毫秒。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ReadRawMagData(HANDLE icdev, int mode, int time_ms, int *rlen, unsigned char *rdata);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_GetEnPassAddr(HANDLE icdev, unsigned char ctime, unsigned char panlen, unsigned char *pandata, unsigned char keylen, unsigned char *keydata, unsigned char *rlen, unsigned char *rdata);

  /**
   * @brief  防拆状态查询。
   * @par    说明：
   * 防拆状态查询。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 防拆机制，目前固定为0x01。
   * @param[in] in_data 输入数据。
   * @param[in] in_len 输入数据的长度。
   * @param[out] out_data 输出数据。
   * @param[out] out_len 输出数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetTamperStatus(HANDLE icdev, unsigned char mode, const unsigned char *in_data, int in_len, unsigned char *out_data, int *out_len);

  /**
   * @brief  防拆侦测设置。
   * @par    说明：
   * 防拆侦测设置。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 防拆机制，目前固定为0x01。
   * @param[in] item 设置项目，0x01表示启动防拆。
   * @param[in] in_data 输入数据。
   * @param[in] in_len 输入数据的长度。
   * @param[out] out_data 输出数据。
   * @param[out] out_len 输出数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SetTamperSense(HANDLE icdev, unsigned char mode, unsigned char item, const unsigned char *in_data, int in_len, unsigned char *out_data, int *out_len);

  /**
   * @brief  获取外部模块版本号。
   * @par    说明：
   * 获取外部模块版本号。
   * @param[in] icdev 设备标识符。
   * @param[in] number 模块序号，从0开始编号。
   * @param[out] version 返回的版本字符串，请至少分配128个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetModuleVersion(HANDLE icdev, int number, char *version);

  /**
   * @brief  SSL证书操作。
   * @par    说明：
   * 直接对网络模块内部文件进行操作。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 操作模式。
   * @n 0x00 - 证书下载。
   * @n 0x01 - 证书查询。
   * @n 0x02 - 证书删除。
   * @param[in] type 当操作模式为证书下载时有效。
   * @n 0x00 - 证书下载初始化。
   * @n 0x01 - 证书下载。
   * @n 0x02 - 证书下载结束。
   * @param[in] in_data 输入数据。
   * @n 当mode==0x00时，如果type==0x00传递下发证书存储路径，type==0x01传递证书实际数据，type==0x02不存在。
   * @n 当mode==0x01时，不存在。
   * @n 当mode==0x02时，证书存储的路径。
   * @param[in] in_len 输入数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SslCertificateControl(HANDLE icdev, unsigned char mode, unsigned char type, const unsigned char *in_data, int in_len);

  /**
   * @brief  网络参数配置。
   * @par    说明：
   * 配置网络参数(TCP(S)/HTTP(S))。
   * @param[in] icdev 设备标识符。
   * @param[in] channel_number 网络通道。
   * @n 0x00 - TCP(s)通道。
   * @n 0x01 - HTTP(s)通道。
   * @param[in] type 网络协议类型。
   * @n 0x00 - TCP。
   * @n 0x01 - SSL。
   * @n 0x02 - HTTP。
   * @n 0x03 - HTTPS。
   * @param[in] mode 工作模式。
   * @n 当type==0x00或type==0x01时，0x01表示长链接，0x02表示短链接。
   * @n 当type==0x02或type==0x03时，0x00表示POST方式，0x01表示GET方式。
   * @param[in] link_timeout_s 链接服务器超时时间，单位为秒。
   * @param[in] response_timeout_s 接受响应超时时间，单位为秒。
   * @param[in] long_link_time_s 长链接保持时间，单位为秒。
   * @param[in] ip_name 服务器的IP或域名地址，如："192.168.10.123"或"www.baidu.com"。
   * @param[in] ip_port IP端口号。
   * @param[in] root_cert_path 根证书路径，为""表示不需要。
   * @param[in] client_cert_path 客户端证书路径，为""表示不需要。
   * @param[in] client_key_path 客户端秘钥路径，为""表示不需要。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_NetParameterConfig(HANDLE icdev, unsigned char channel_number, unsigned char type, unsigned char mode, unsigned char link_timeout_s, unsigned char response_timeout_s, unsigned int long_link_time_s, const char *ip_name, unsigned short ip_port, const char *root_cert_path, const char *client_cert_path, const char *client_key_path);

  /**
   * @brief  打开网络链接。
   * @par    说明：
   * 打开网络链接。
   * @param[in] icdev 设备标识符。
   * @param[in] channel_number 网络通道。
   * @n 0x00 - TCP(s)通道。
   * @n 0x01 - HTTP(s)通道。
   * @param[in] reserved 保留，固定为0x00。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_OpenNetLink(HANDLE icdev, unsigned char channel_number, unsigned char reserved);

  /**
   * @brief  关闭网络链接。
   * @par    说明：
   * 关闭网络链接。
   * @param[in] icdev 设备标识符。
   * @param[in] channel_number 网络通道。
   * @n 0x00 - TCP(s)通道。
   * @n 0x01 - HTTP(s)通道。
   * @param[in] reserved 保留，固定为0x00。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_CloseNetLink(HANDLE icdev, unsigned char channel_number, unsigned char reserved);

  /**
   * @brief  向服务器发送数据。
   * @par    说明：
   * 向服务器发送数据。
   * @param[in] icdev 设备标识符。
   * @param[in] channel_number 网络通道。
   * @n 0x00 - TCP(s)通道。
   * @n 0x01 - HTTP(s)通道。
   * @param[in] in_data 输入数据。
   * @param[in] in_len 输入数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SendDataToServer(HANDLE icdev, unsigned char channel_number, const unsigned char *in_data, int in_len);

  /**
   * @brief  从服务器接收数据。
   * @par    说明：
   * 从服务器接收数据。
   * @param[in] icdev 设备标识符。
   * @param[in] channel_number 网络通道。
   * @n 0x00 - TCP(s)通道。
   * @n 0x01 - HTTP(s)通道。
   * @param[in,out] state 传入陈述并且返回状态，传入时：0x00表示启动，0x01表示读数据，0x02表示退出；返回时：0x00表示还没有接收到服务器数据，0x01表示模块还有多余数据，0x02模块数据取完了。
   * @param[out] out_data 输出数据。
   * @param[out] out_len 输出数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_RecvDataFromServer(HANDLE icdev, unsigned char channel_number, unsigned char *state, unsigned char *out_data, int *out_len);

  /**
   * @brief  设置读写器型号。
   * @par    说明：
   * 设置读写器型号。
   * @param[in] icdev 设备标识符。
   * @param[in] model 传入型号，数据格式为'\0'结尾的字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SetReaderModel(HANDLE icdev, const char *model);

  /**
   * @brief  获取读写器型号。
   * @par    说明：
   * 获取读写器型号。
   * @param[in] icdev 设备标识符。
   * @param[out] model 返回的型号字符串，请至少分配33个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetReaderModel(HANDLE icdev, char *model);

  /**
   * @brief  启动读磁条卡。
   * @par    说明：
   * 使设备进入监测刷磁条的状态，接口本身会直接返回，而不会等待刷完磁条卡。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 模式。
   * @n 0x01 - 表示磁卡原始数据流正反向解码，后续使用 ::dc_readmag2 获取磁条卡数据。
   * @n 0x02 - 表示磁卡7811协议解码数据，后续使用 ::dc_readmag 获取磁条卡数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_startreadmag2(HANDLE icdev, unsigned char mode);

  /**
   * @brief  获取磁条卡数据。
   * @par    说明：
   * 获取设备已经读取到的磁条卡数据。
   * @param[in] icdev 设备标识符。
   * @param[out] t1pp_data 返回的1轨道同向正解数据。
   * @param[out] t1pp_len 返回1轨道同向正解数据的长度。
   * @param[out] t1pn_data 返回的1轨道同向反解数据。
   * @param[out] t1pn_len 返回1轨道同向反解数据的长度。
   * @param[out] t1np_data 返回的1轨道反向正解数据。
   * @param[out] t1np_len 返回1轨道同反正解数据的长度。
   * @param[out] t1nn_data 返回的1轨道反向反解数据。
   * @param[out] t1nn_len 返回1轨道反向反解数据的长度。
   * @param[out] t2pp_data 返回的2轨道同向正解数据。
   * @param[out] t2pp_len 返回2轨道同向正解数据的长度。
   * @param[out] t2pn_data 返回的2轨道同向反解数据。
   * @param[out] t2pn_len 返回2轨道同向反解数据的长度。
   * @param[out] t2np_data 返回的2轨道反向正解数据。
   * @param[out] t2np_len 返回2轨道同反正解数据的长度。
   * @param[out] t2nn_data 返回的2轨道反向反解数据。
   * @param[out] t2nn_len 返回2轨道反向反解数据的长度。
   * @param[out] t3pp_data 返回的3轨道同向正解数据。
   * @param[out] t3pp_len 返回3轨道同向正解数据的长度。
   * @param[out] t3pn_data 返回的3轨道同向反解数据。
   * @param[out] t3pn_len 返回3轨道同向反解数据的长度。
   * @param[out] t3np_data 返回的3轨道反向正解数据。
   * @param[out] t3np_len 返回3轨道同反正解数据的长度。
   * @param[out] t3nn_data 返回的3轨道反向反解数据。
   * @param[out] t3nn_len 返回3轨道反向反解数据的长度。
   * @return <0表示失败，==0表示成功，==1表示等待刷卡，==2表示处于停止刷卡状态。
   */
  API_ATTRIBUTE short USER_API dc_readmag2(HANDLE icdev, unsigned char *t1pp_data, unsigned int *t1pp_len, unsigned char *t1pn_data, unsigned int *t1pn_len, unsigned char *t1np_data, unsigned int *t1np_len, unsigned char *t1nn_data, unsigned int *t1nn_len, unsigned char *t2pp_data, unsigned int *t2pp_len, unsigned char *t2pn_data, unsigned int *t2pn_len, unsigned char *t2np_data, unsigned int *t2np_len, unsigned char *t2nn_data, unsigned int *t2nn_len, unsigned char *t3pp_data, unsigned int *t3pp_len, unsigned char *t3pn_data, unsigned int *t3pn_len, unsigned char *t3np_data, unsigned int *t3np_len, unsigned char *t3nn_data, unsigned int *t3nn_len);

  /**
   * @brief  电动读卡器EM3初始化。
   * @par    说明：
   * 电动读卡器EM3初始化。
   * @param[in] icdev 设备标识符。
   * @param[in] gate_check_mode 前门电闸门检测模式。
   * @n 0x00 - 表示不检测。
   * @n 0x01 - 表示检测。
   * @param[in] card_move_mode 初始化如内部有卡时卡移动模式。
   * @n 0x00 - 弹卡到前门夹卡状态。
   * @n 0x01 - 弹卡到前门不夹卡状态。
   * @n 0x02 - 吞卡。
   * @n 0x03 - 卡移动到卡机内部。
   * @param[in] request_mode 卡机功能模块类型识别码返回控制模式。
   * @n 0x00 - 表示不返回卡机功能模块类型识别码。
   * @n 0x01 - 表示返回卡机功能模块类型识别码。
   * @param[out] out_data 输出数据，当存在时含义如下。
   * @n byte0 - SAM1卡座是否有卡，0x00表示无卡，0x01表示有卡。
   * @n byte1 - SAM2卡座是否有卡，0x00表示无卡，0x01表示有卡。
   * @n byte2 - SAM3卡座是否有卡，0x00表示无卡，0x01表示有卡。
   * @n byte3 - SAM4卡座是否有卡，0x00表示无卡，0x01表示有卡。
   * @n byte4 - 硬件是否有IC检测装置，0x00表示无，0x01表示有。
   * @n byte5 - 硬件是否有防钓鱼装置，0x00表示无，0x01表示有。
   * @n byte6 - 硬件是否有磁卡干扰装置，0x00表示无，0x01表示有。
   * @n byte7 - 硬件是否有身份证模块装置，0x00表示无，0x01表示有。
   * @param[out] out_len 输出数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SelfServiceDeviceInit(HANDLE icdev, unsigned char gate_check_mode, unsigned char card_move_mode, unsigned char request_mode, unsigned char *out_data, int *out_len);

  /**
   * @brief  电动读卡器磁条写卡速度控制。
   * @par    说明：
   * 电动读卡器磁条写卡速度控制。
   * @param[in] icdev 设备标识符。
   * @param[in] value 速度值，范围1~99，值越大越快。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SelfServiceDeviceWriteMagSpeed(HANDLE icdev, unsigned char value);

  /**
   * @brief  控制脉冲信号输出。
   * @par    说明：
   * 控制脉冲信号输出。
   * @param[in] icdev 设备标识符。
   * @param[in] on_off 控制开启或者关闭，0x00表示关闭，0x01表示开启。
   * @param[in] occupation_ratio 占空比，百分比范围0~100。
   * @param[in] frequency 脉冲频率，单位为Hz。
   * @param[in] output_time_ms 脉冲输出的时间，单位为毫秒，0表示一直输出。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_PulseSignalControl(HANDLE icdev, unsigned char on_off, unsigned char occupation_ratio, unsigned int frequency, unsigned int output_time_ms);

  /**
   * @brief  获取调试信息。
   * @par    说明：
   * 获取调试信息。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类型，0x00表示日志，0x01表示Shell指令，0x02表示读文件，0x03表示写文件。
   * @param[in] time_100ms 设备超时值，单位为100毫秒。
   * @param[in] slen 发送数据的长度。
   * @param[in] sdata 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetDeviceDebugInfo(HANDLE icdev, unsigned char type, int time_100ms, int slen, const unsigned char *sdata, int *rlen, unsigned char *rdata);

  /**
   * @brief  获取温度。
   * @par    说明：
   * 获取温度。
   * @param[in] icdev 设备标识符。
   * @param[out] average_value 平均温度。
   * @param[out] maximum_value 最高温度。
   * @param[out] minimum_value 最低温度。
   * @param[out] ambient_value 环境温度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_TakeTemperatureValues(HANDLE icdev, unsigned short *average_value, unsigned short *maximum_value, unsigned short *minimum_value, unsigned short *ambient_value);

  /**
   * @brief  外部模块电源控制。
   * @par    说明：
   * 用于对外部模块的电源进行上电或下电控制。
   * @param[in] icdev 设备标识符。
   * @param[in] module_number 模块编号。
   * @n 0x0001 - 基础外设，如：SPI、FLASH、蜂鸣器、指示灯等。
   * @n 0x0101 - Android模块。
   * @param[in] value 值，0x00表示下电，0x01表示上电。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ModulePowerControl(HANDLE icdev, unsigned short module_number, unsigned char value);

  /**
   * @brief  外部模块心跳包控制。
   * @par    说明：
   * 用于开启或关闭检测外部模块心跳包功能，以便设备知晓外设模块是否仍在正常运行。
   * @param[in] icdev 设备标识符。
   * @param[in] module_number 模块编号。
   * @n 0x0001 - 基础外设，如：SPI、FLASH、蜂鸣器、指示灯等。
   * @n 0x0101 - Android模块。
   * @param[in] value 值，0x00表示关闭，0x01表示开启。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ModuleHeartbeatPacketControl(HANDLE icdev, unsigned short module_number, unsigned char value);

  /**
   * @brief  通知主业务APP运行状态。
   * @par    说明：
   * 主业务APP用此接口来告知设备当前自己的运行状态。
   * @param[in] icdev 设备标识符。
   * @param[in] value 值，0x00表示APP退出，0x01表示APP启动。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_NotifyMainAppState(HANDLE icdev, unsigned char value);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API DC_GetCard_CsnCarId(HANDLE icdev, char *cCardNo, char *cCardType);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API DC_GetCard_Check(HANDLE icdev, char *cIn, char *cOut);

  /**
   * @brief  字符串转换。
   * @par    说明：
   * 字符串转换。
   * @param[in] type 类型。
   * @n 0 - GBK->UCS-2LE。
   * @n 1 - UCS-2LE->GBK。
   * @n 2 - GBK->UTF-8。
   * @n 3 - UTF-8->GBK。
   * @n 4 - UCS-2LE->UTF-8。
   * @n 5 - UTF-8->UCS-2LE。
   * @param[in] in_str 输入字符串。
   * @param[out] out_str 输出字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_string_converter(int type, const unsigned char *in_str, unsigned char *out_str);

  /**
   * @brief  虚拟键盘通讯。
   * @par    说明：
   * 通过此接口调用，传入信息，使得设备虚拟键盘接口（复合键盘设备接口）输出相应的数据。
   * @param[in] icdev 设备标识符。
   * @param[in] param_data 输入参数数据，由参数数据的长度决定存在哪些参数，不存在的参数使用设备配置值，不存在配置值将使用缺省值，下面列出存在时的定义。
   * @n param_data[0] - 自动上传是否带有回车键，0表示不带回车键（缺省值），1表示带回车键。
   * @n param_data[1] - 自动上传数据模式，0表示不拆分模式（缺省值），1表示大写拆分模式（即十六进制大写字符数据），2表示小写拆分模式（即十六进制小写字符数据）。
   * @param[in] param_len 输入参数数据的长度。
   * @param[in] key_data 输入键值数据。
   * @param[in] key_len 输入键值数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_VirtualKeyOutputControl(HANDLE icdev, const unsigned char *param_data, unsigned char param_len, const unsigned char *key_data, unsigned short key_len);

  /**
   * @brief  Android电源定时控制。
   * @par    说明：
   * Android电源定时控制。
   * @param[in] icdev 设备标识符。
   * @param[in] power_switch 电源开关，0x00表示下电，0x01表示上电。
   * @param[in] time_s 定时时间，单位为秒。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_AndroidTimingControl(HANDLE icdev, unsigned char power_switch, unsigned char time_s);

  /**
   * @brief  读取串口波特率。
   * @par    说明：
   * 读取串口波特率，非永久存储值。
   * @param[in] icdev 设备标识符。
   * @param[out] value 波特率。
   * @n 0x00 - 115200。
   * @n 0x01 - 57600。
   * @n 0x02 - 38400。
   * @n 0x03 - 19200。
   * @n 0x04 - 9600。
   * @n 0x05 - 4800。
   * @n 0x06 - 2400。
   * @n 0x07 - 1200。
   * @n 0x08 - 460800。
   * @n 0x09 - 128000。
   * @n 0x0A - 230400。
   * @n 0x0B - 256000。
   * @n 0x0C - 921600。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetCommBaudrate(HANDLE icdev, unsigned char *value);

  /**
   * @brief  设置串口波特率。
   * @par    说明：
   * 设置串口波特率，非永久存储值。
   * @param[in] icdev 设备标识符。
   * @param[in] value 波特率。
   * @n 0x00 - 115200。
   * @n 0x01 - 57600。
   * @n 0x02 - 38400。
   * @n 0x03 - 19200。
   * @n 0x04 - 9600。
   * @n 0x05 - 4800。
   * @n 0x06 - 2400。
   * @n 0x07 - 1200。
   * @n 0x08 - 460800。
   * @n 0x09 - 128000。
   * @n 0x0A - 230400。
   * @n 0x0B - 256000。
   * @n 0x0C - 921600。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SetCommBaudrate(HANDLE icdev, unsigned char value);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * 对当前卡座CPU卡进行指令交互操作，注意此接口已封装卡协议部分并且强制使用指定卡协议进行交互。
   * @param[in] icdev 设备标识符。
   * @param[in] type 卡片协议。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_TransmitApdu(HANDLE icdev, unsigned char type, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * ::dc_TransmitApdu 的HEX形式接口，参数 @a sendbuffer @a databuffer 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_TransmitApdu_hex(HANDLE icdev, unsigned char type, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer);

  /**
   * @brief  写TLV用户配置。
   * @par    说明：
   * 写TLV用户配置。
   * @param[in] icdev 设备标识符。
   * @param[in] tlv_data_len TLV数据的长度。
   * @param[in] tlv_data TLV数据。
   * @param[out] tag_count Tag数量。
   * @param[out] tag_info Tag信息。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_WriteTlvUserConfig(HANDLE icdev, unsigned int tlv_data_len, const unsigned char *tlv_data, int *tag_count, unsigned int *tag_info);

  /**
   * @brief  读TLV用户配置。
   * @par    说明：
   * 读TLV用户配置。
   * @param[in] icdev 设备标识符。
   * @param[in] tag_count Tag数量。
   * @param[in] tag_info Tag信息。
   * @param[out] tlv_data_len TLV数据的长度。
   * @param[out] tlv_data TLV数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ReadTlvUserConfig(HANDLE icdev, int tag_count, const unsigned int *tag_info, unsigned int *tlv_data_len, unsigned char *tlv_data);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_RfRequestTest(HANDLE icdev, unsigned char type);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_SamAGetSAMStatus(HANDLE icdev);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_SamAGetSAMID(HANDLE icdev, unsigned char *sam_id);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_SamAGetSAMCert(HANDLE icdev, unsigned char *cert_data, unsigned int *cert_len);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_SamAEnableSAM(HANDLE icdev, const unsigned char *info_data, unsigned int info_len);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_SamADisableSAM(HANDLE icdev, const unsigned char *info_data, unsigned int info_len);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_SamAGetEnaStat(HANDLE icdev, unsigned char *state, unsigned int *remain_number, unsigned char *enable_time, unsigned char *disable_time, unsigned char *second_cert, unsigned int *second_cert_len, unsigned char *encode_cert, unsigned int *encode_cert_len, unsigned char *sign_cert, unsigned int *sign_cert_len);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_SamAStartFindIDCard(HANDLE icdev, unsigned char *management_number);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_SamASelectIDCard(HANDLE icdev, unsigned char *serial_number);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_SamAReadChkData(HANDLE icdev, const unsigned char *r_data, unsigned char *t_data, unsigned char *shortcode, unsigned char *data_with_sign, unsigned int *data_with_sign_len);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_SamAReadChkDataPF(HANDLE icdev, const unsigned char *r_data, unsigned char *t_data, unsigned char *shortcode, unsigned char *data_with_sign, unsigned int *data_with_sign_len, unsigned char *photo, unsigned int *photo_len, unsigned char *fingerprint, unsigned int *fingerprint_len);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_SamACheckSLIdencode(HANDLE icdev, const unsigned char *r_data, unsigned char short_number, const unsigned char *shortcode, unsigned char long_number, const unsigned char *longcode, unsigned char *code_len, unsigned char *matchcode, unsigned char *t_data, unsigned char *data_with_sign, unsigned int *data_with_sign_len);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_SamACheckSLIdencodePF(HANDLE icdev, const unsigned char *r_data, unsigned char short_number, const unsigned char *shortcode, unsigned char long_number, const unsigned char *longcode, unsigned char *code_len, unsigned char *matchcode, unsigned char *t_data, unsigned char *data_with_sign, unsigned int *data_with_sign_len, unsigned char *photo, unsigned int *photo_len, unsigned char *fingerprint, unsigned int *fingerprint_len);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_SamACheckGenderData(HANDLE icdev, const unsigned char *r_data, const unsigned char *gender, unsigned char *result_data, unsigned char *data_with_sign, unsigned int *data_with_sign_len);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_SamACheckBirthdateData(HANDLE icdev, const unsigned char *r_data, const unsigned char *birthdate, unsigned char *result_data, unsigned char *data_with_sign, unsigned int *data_with_sign_len);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_SamAInit(HANDLE icdev);

  /**
   * @brief  获取测温模块信息。
   * @par    说明：
   * 获取测温模块信息。
   * @param[in] icdev 设备标识符。
   * @param[in] time_100ms 与测温模块通信的超时等待时间，单位为100毫秒。
   * @param[in] tag Tag信息。
   * @n 0xFF - 请求获取所有Tag信息。
   * @n 0x01 - 模块型号。
   * @n 0x02 - 模块序列号。
   * @n 0x03 - 模块固件版本。
   * @param[out] tlv_data_len TLV数据的长度。
   * @param[out] tlv_data TLV数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetTemperatureModuleInfo(HANDLE icdev, int time_100ms, unsigned char tag, unsigned int *tlv_data_len, unsigned char *tlv_data);

  /**
   * @brief  获取温度信息。
   * @par    说明：
   * 获取温度信息。
   * @param[in] icdev 设备标识符。
   * @param[in] time_100ms 与测温模块通信的超时等待时间，单位为100毫秒。
   * @param[in] tag Tag信息。
   * @n 0xFF - 请求获取所有Tag信息。
   * @n 0x01 - 环境温度。
   * @n 0x02 - 测试标的温度。
   * @n 0x03 - 获取校准值。
   * @param[out] tlv_data_len TLV数据的长度。
   * @param[out] tlv_data TLV数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetTemperatureInfo(HANDLE icdev, int time_100ms, unsigned char tag, unsigned int *tlv_data_len, unsigned char *tlv_data);

  /**
   * @brief  设置温度传感器参数。
   * @par    说明：
   * 设置温度传感器参数。
   * @param[in] icdev 设备标识符。
   * @param[in] time_100ms 与测温模块通信的超时等待时间，单位为100毫秒。
   * @param[in] tag Tag信息。
   * @n 0x03 - 获取校准值。
   * @param[in] data_len 数据的长度。
   * @param[in] data 数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SetTemperatureInfo(HANDLE icdev, int time_100ms, unsigned char tag, unsigned int data_len, const unsigned char *data);

  /**
   * @brief  获取接近感应模块信息。
   * @par    说明：
   * 获取接近感应模块信息。
   * @param[in] icdev 设备标识符。
   * @param[in] time_100ms 与测温模块通信的超时等待时间，单位为100毫秒。
   * @param[in] number 接近感应传感器编号。
   * @param[in] tag Tag信息。
   * @n 0xFF - 请求获取所有Tag信息。
   * @n 0x01 - 模块型号。
   * @n 0x04 - 获取PS传感器值。
   * @param[out] tlv_data_len TLV数据的长度。
   * @param[out] tlv_data TLV数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetProximitySensorInfo(HANDLE icdev, int time_100ms, unsigned char number, unsigned char tag, unsigned int *tlv_data_len, unsigned char *tlv_data);

  /**
   * @brief  非接触式CPU卡复位（复位信息为ATR）。
   * @par    说明：
   * 对感应区CPU卡进行复位操作。
   * @param[in] icdev 设备标识符。
   * @param[out] rlen 返回复位信息的长度。
   * @param[out] rdata 返回的复位信息，请至少分配128个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_RfCpuResetAtr(HANDLE icdev, unsigned char *rlen, unsigned char *rdata);

  /**
   * @brief  透传通讯。
   * @par    说明：
   * 透传通讯。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类型，0x00表示P3键盘，0x01表示Android。
   * @param[in] time_100ms 设备超时值，单位为100毫秒。
   * @param[in] flag 标志，固定为2个字节，可以解析为一个无符号整数（Big-Endian），值为下面列出的一种或多种之和。
   * @n 0x0001 - 需要先清理通讯缓存。
   * @param[in] slen 发送数据的长度。
   * @param[in] sdata 发送数据。
   * @param[in] elen 期待数据的长度。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_PPCommData(HANDLE icdev, unsigned char type, int time_100ms, const unsigned char *flag, int slen, const unsigned char *sdata, int elen, int *rlen, unsigned char *rdata);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_ExternalIoControl(HANDLE icdev, unsigned char direction, const unsigned char *sdata, int slen, unsigned char *rdata, int *rlen, int time_ms);

  /**
   * @brief  接触式CPU卡指令交互。
   * @par    说明：
   * 对当前卡座CPU卡进行指令交互操作，注意此接口已封装卡协议部分并且使用加密方式进行通讯。
   * @param[in] icdev 设备标识符。
   * @param[in] reserved 保留，固定为8个字节。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_AlgorithmApdu(HANDLE icdev, const unsigned char *reserved, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer);

  /**
   * @brief  非接触式CPU卡指令交互。
   * @par    说明：
   * 对当前卡座CPU卡进行指令交互操作，注意此接口已封装卡协议部分并且使用加密方式进行通讯。
   * @param[in] icdev 设备标识符。
   * @param[in] reserved 保留，固定为8个字节。
   * @param[in] slen 发送数据的长度。
   * @param[in] sendbuffer 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] databuffer 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_AlgorithmRfApdu(HANDLE icdev, const unsigned char *reserved, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_GetNfcCardEmulationParameter(HANDLE icdev, unsigned char tag, unsigned int *tlv_data_len, unsigned char *tlv_data);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_SetNfcCardEmulationParameter(HANDLE icdev, unsigned char tag, unsigned int data_len, const unsigned char *data);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_SendNfcCardEmulationData(HANDLE icdev, unsigned char type, unsigned char flag, unsigned int data_len, const unsigned char *data);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_RecvNfcCardEmulationData(HANDLE icdev, unsigned char type, unsigned char *flag, unsigned int *data_len, unsigned char *data);

  /**
   * @brief  指示灯闪烁控制。
   * @par    说明：
   * 控制设备的指示灯。
   * @param[in] icdev 设备标识符。
   * @param[in] number 指示灯编号，0表示全部指示灯，1表示第一个指示灯，2表示第二个指示灯，以此类推。
   * @param[in] on_time_ms 指示灯亮的时间，单位为毫秒。
   * @param[in] off_time_ms 指示灯灭的时间，单位为毫秒。
   * @param[in] count 闪烁次数，0表示停止闪烁，0xFFFF表示一直闪烁。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_LedFlickerControl(HANDLE icdev, unsigned char number, int on_time_ms, int off_time_ms, int count);

  /**
   * @brief  获取网卡信息。
   * @par    说明：
   * 获取网卡信息。
   * @param[in] icdev 设备标识符。
   * @param[in] name 网卡名称字符串，为""表示获取所有网卡信息。
   * @param[out] info_data 返回的网卡信息，请至少分配1024个字节。
   * @param[out] info_len 返回网卡信息的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetNicInfo(HANDLE icdev, const char *name, unsigned char *info_data, int *info_len);

  /**
   * @brief  选择通讯网卡。
   * @par    说明：
   * 选择通讯网卡。
   * @param[in] icdev 设备标识符。
   * @param[in] name 网卡名称字符串。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SelectCommunicationNic(HANDLE icdev, const char *name);

  /**
   * @brief  获取设备模块信息。
   * @par    说明：
   * 获取设备模块信息。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sdata 发送数据。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetDeviceModuleInfo(HANDLE icdev, int slen, const unsigned char *sdata, int *rlen, unsigned char *rdata);

  /**
   * @brief  打开安全芯片通道。
   * @par    说明：
   * 打开安全芯片通道。
   * @param[in] icdev 设备标识符。
   * @param[in] type 通道类型。
   * @n 0x00 - 基础通道。
   * @n 0x01 - 逻辑通道。
   * @param[in] parameter 控制参数。
   * @param[in] isd_data ISD数据。
   * @param[in] isd_len ISD数据的长度。
   * @param[out] response_data 响应的数据。
   * @param[out] response_len 响应数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SecurityChipOpen(HANDLE icdev, unsigned char type, unsigned char parameter, const unsigned char *isd_data, unsigned int isd_len, unsigned char *response_data, unsigned int *response_len);

  /**
   * @brief  安全芯片透传通讯。
   * @par    说明：
   * 安全芯片透传通讯。
   * @param[in] icdev 设备标识符。
   * @param[in] request_data 请求数据。
   * @param[in] request_len 请求数据的长度。
   * @param[out] response_data 响应的数据。
   * @param[out] response_len 响应数据的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SecurityChipCommunication(HANDLE icdev, const unsigned char *request_data, unsigned int request_len, unsigned char *response_data, unsigned int *response_len);

  /**
   * @brief  关闭安全芯片通道。
   * @par    说明：
   * 关闭安全芯片通道。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SecurityChipClose(HANDLE icdev);

  /**
   * @brief  NTAG密码校验。
   * @par    说明：
   * NTAG密码校验。
   * @param[in] icdev 设备标识符。
   * @param[in] key 密码，固定为4个字节。
   * @param[out] pack 返回的PACK，固定为2个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_pwd_auth(HANDLE icdev, const unsigned char *key, unsigned char *pack);

  /**
   * @brief  NTAG密码校验。
   * @par    说明：
   * ::dc_pwd_auth 的HEX形式接口，参数 @a key @a pack 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_pwd_auth_hex(HANDLE icdev, const char *key, char *pack);

  /**
   * @brief  NTAG读卡数据。
   * @par    说明：
   * NTAG读卡数据。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 保留，固定为0xFF。
   * @param[in] length 数据长度。
   * @param[in] addr 地址。
   * @param[out] data 数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_read2(HANDLE icdev, unsigned char flag, unsigned char length, int addr, unsigned char *data);

  /**
   * @brief  NTAG读卡数据。
   * @par    说明：
   * ::dc_read2 的HEX形式接口，参数 @a data 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_read2_hex(HANDLE icdev, unsigned char flag, unsigned char length, int addr, char *data);

  /**
   * @brief  NTAG写卡数据。
   * @par    说明：
   * NTAG写卡数据。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 保留，固定为0xFF。
   * @param[in] length 数据长度。
   * @param[in] addr 地址。
   * @param[in] data 数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_write2(HANDLE icdev, unsigned char flag, unsigned char length, int addr, const unsigned char *data);

  /**
   * @brief  NTAG写卡数据。
   * @par    说明：
   * ::dc_write2 的HEX形式接口，参数 @a data 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_write2_hex(HANDLE icdev, unsigned char flag, unsigned char length, int addr, const char *data);

  /**
   * @brief  验证Mifare Ultralight AES卡密码。
   * @par    说明：
   * 使用传入的密码来验证Mifare Ultralight AES卡密码。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 保留，固定为0xFF。
   * @param[in] arg 密钥的类型。
   * @n 0x00 - DataProtKey。
   * @n 0x01 - UIDRetrKey。
   * @n 0x02 - OriginalityKey。
   * @param[in] authkey 密码，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_auth_aes(HANDLE icdev, unsigned char flag, unsigned char arg, const unsigned char *authkey);

  /**
   * @brief  验证Mifare Ultralight AES卡密码。
   * @par    说明：
   * ::dc_auth_aes 的HEX形式接口，参数 @a authkey 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_auth_aes_hex(HANDLE icdev, unsigned char flag, unsigned char arg, const char *authkey);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_aesblock(int direct, const unsigned char *chiper_data_buffer, unsigned char data_len, const unsigned char *key, const unsigned char *iv_data, unsigned char *result_buffer);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_aesblock_hex(int direct, const char *chiper_data_buffer, unsigned char data_len, const char *key, const char *iv_data, char *result_buffer);

  /**
   * @brief  获取ISO14443属性。
   * @par    说明：
   * 获取ISO14443属性，此接口在一些ISO14443相关函数调用后可以获取。
   * @param[in] icdev 设备标识符。
   * @param[in] type 属性类别。
   * @n 1 - 获取ATQA。
   * @n 2 - 获取SAK。
   * @n 3 - 获取ATS。
   * @param[out] data 数据。
   * @param[in,out] data_size 数据大小。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetIso14443Attribute(HANDLE icdev, int type, unsigned char *data, unsigned int *data_size);

  /**
   * @brief  寻卡请求、防卡冲突、选卡操作。
   * @par    说明：
   * 内部包含了 ::dc_request ::dc_anticoll ::dc_select ::dc_anticoll2 ::dc_select2 ::dc_anticoll3 ::dc_select3 的功能。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 模式，同 ::dc_request 的 @a _Mode 。
   * @param[out] uid_len 返回卡序列号的长度。
   * @param[out] uid 返回的卡序列号，请至少分配16个字节。
   * @return <0表示失败，==0表示成功，==1表示无卡或无法寻到卡片。
   */
  API_ATTRIBUTE short USER_API dc_FindCard(HANDLE icdev, unsigned char mode, unsigned int *uid_len, unsigned char *uid);

  /**
   * @brief  请求RATS操作。
   * @par    说明：
   * 请求RATS操作。
   * @param[in] icdev 设备标识符。
   * @param[in] fsdi FSDI，FSDI和FSD对应关系如下。
   * @n 0 - 16字节。
   * @n 1 - 24字节。
   * @n 2 - 32字节。
   * @n 3 - 40字节。
   * @n 4 - 48字节。
   * @n 5 - 64字节。
   * @n 6 - 96字节。
   * @n 7 - 128字节。
   * @n 8 - 256字节。
   * @param[in] cid CID，取值范围为0x00~0x0F。
   * @param[out] ats_len 返回ATS数据的长度。
   * @param[out] ats 返回的ATS数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_RequestRATS(HANDLE icdev, unsigned char fsdi, unsigned char cid, unsigned int *ats_len, unsigned char *ats);

  /**
   * @brief  请求PPS操作。
   * @par    说明：
   * 请求PPS操作。
   * @param[in] icdev 设备标识符。
   * @param[in] cid CID，取值范围为0x00~0x0F。
   * @param[in] dri DRI，取值范围为0x00~0x03。
   * @param[in] dsi DSI，取值范围为0x00~0x03。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_RequestPPS(HANDLE icdev, unsigned char cid, unsigned char dri, unsigned char dsi);

  /**
   * @brief  设置红外电源模式。
   * @par    说明：
   * 设置红外电源模式，注意：1、当控制模式处于关闭状态时才能开启新的电源，每次开启/关闭时会对补货数量清零。2、当控制模式为关闭并且起点列号和终点列号都为0xFF时表示关闭所有电源。3、M300YR1A项目中，当红外电源开启后，如果在关闭红外电源前设备检测到补货行为，则设备端将主动输出数据。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 控制模式，0x00表示开启，0x01表示关闭。
   * @param[in] line_number 行号，取值范围为0x00~0xFE。
   * @param[in] column_begin_number 起点列号，取值范围为0x00~0xFE。
   * @param[in] column_end_number 终点列号，取值范围为0x00~0xFE。
   * @param[out] shipped_number 补货数量。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_InfraredSetPowerMode(HANDLE icdev, unsigned char mode, unsigned char line_number, unsigned char column_begin_number, unsigned char column_end_number, unsigned char *shipped_number);

  /**
   * @brief  获取红外电源模式。
   * @par    说明：
   * 获取红外电源模式。
   * @param[in] icdev 设备标识符。
   * @param[out] mode 控制模式，0x00表示开启，0x01表示关闭。
   * @param[out] line_number 行号，取值范围为0x00~0xFE。
   * @param[out] column_begin_number 起点列号，取值范围为0x00~0xFE。
   * @param[out] column_end_number 终点列号，取值范围为0x00~0xFE。
   * @param[out] supported_mode 设备支持模式。
   * @n 0x01 - 表示起点列号和终点列号分别整除8所得到的商必须相同。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_InfraredGetPowerMode(HANDLE icdev, unsigned char *mode, unsigned char *line_number, unsigned char *column_begin_number, unsigned char *column_end_number, unsigned char *supported_mode);

  /**
   * @brief  检测红外传感器状态。
   * @par    说明：
   * 检测红外传感器状态，此接口为设备检修指令，只有设备的红外控制电源全部关闭时才能使用。
   * @param[in] icdev 设备标识符。
   * @param[in] line_number 行号，取值范围为0x00~0xFE。
   * @param[in] column_begin_number 起点列号，取值范围为0x00~0xFE。
   * @param[in] column_end_number 终点列号，取值范围为0x00~0xFE。
   * @param[out] status_bytes 传感器状态，使用起点列号到终点列号的状态逐一按字节返回。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_InfraredCheckStatus(HANDLE icdev, unsigned char line_number, unsigned char column_begin_number, unsigned char column_end_number, unsigned char *status_bytes);

  /**
   * @brief  配置红外传感器检测时间。
   * @par    说明：
   * 配置红外传感器检测时间，此接口未使用时，默认为关闭红外电源前自动检测功能一直有效。
   * @param[in] icdev 设备标识符。
   * @param[in] time_s 超时检测时间，单位为秒。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_InfraredConfigCheckTime(HANDLE icdev, unsigned short time_s);

  /**
   * @brief  设备激活配置。
   * @par    说明：
   * 设备激活配置，用于485通讯模式下的多设备激活状态的设置和获取。
   * @param[in] icdev 设备标识符。
   * @param[in,out] activation_mode 激活模式，0x00表示获取设备激活状态，0x01表示设置设备为激活模式，0x02表示设置设备为未激活模式。
   * @param[in] work_mode 工作模式，0x00表示用户模式，0xFF表示维护模式（连接设备只能是1台）。
   * @param[in] uid 设备唯一识别码字符串，使用维护模式时可以传入空字符串，即：""。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_DeviceActivationConfig(HANDLE icdev, unsigned char *activation_mode, unsigned char work_mode, const char *uid);

  /**
   * @brief  配置激活UID。
   * @par    说明：
   * 配置激活UID，在调用 ::dc_init 之前可以使用此接口来配置激活的485设备，用于连接设备，连接后如需切换设备将使用 ::dc_DeviceActivationConfig 而不是本接口。
   * @param[in] work_mode 工作模式，0x00表示用户模式，0xFF表示维护模式（连接设备只能是1台）。
   * @param[in] uid 设备唯一识别码字符串，使用维护模式时可以传入空字符串，即：""。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ConfigActivationUid(unsigned char work_mode, const char *uid);

  /**
   * @brief  主机主动获取透传数据。
   * @par    说明：
   * 主机主动获取透传数据。
   * @param[in] icdev 设备标识符。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_HostGetRequestData(HANDLE icdev, int *rlen, unsigned char *rdata);

  /**
   * @brief  主机主动响应透传数据。
   * @par    说明：
   * 主机主动响应透传数据。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sdata 发送数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_HostSetResponseData(HANDLE icdev, int slen, const unsigned char *sdata);

  /**
   * @brief  配置GPIO模式。
   * @par    说明：
   * 配置GPIO模式。
   * @param[in] icdev 设备标识符。
   * @param[in] pin_number 引脚号。
   * @param[in] work_mode 工作模式，0x01表示上拉输入模式，0x02表示推挽输出模式。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ConfigGpioMode(HANDLE icdev, unsigned char pin_number, unsigned char work_mode);

  /**
   * @brief  写GPIO。
   * @par    说明：
   * 写GPIO。
   * @param[in] icdev 设备标识符。
   * @param[in] pin_number 引脚号。
   * @param[in] value GPIO值，0或1。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_WriteGpio(HANDLE icdev, unsigned char pin_number, unsigned char value);

  /**
   * @brief  读GPIO。
   * @par    说明：
   * 读GPIO。
   * @param[in] icdev 设备标识符。
   * @param[in] pin_number 引脚号。
   * @param[out] value GPIO值，0或1。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ReadGpio(HANDLE icdev, unsigned char pin_number, unsigned char *value);

  /**
   * @brief  获取电池信息。
   * @par    说明：
   * 获取电池信息。
   * @param[in] icdev 设备标识符。
   * @param[in] tag Tag信息。
   * @n 0xFF - 请求获取所有Tag信息。
   * @n 0x01 - 适配器是否插入。TLV值部分可能为0x00或0x01，0x00表示无适配器，0x01表示有适配器。
   * @n 0x02 - 电池电压。
   * @n 0x03 - 电池电量百分比。
   * @param[out] tlv_data_len TLV数据的长度。
   * @param[out] tlv_data TLV数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetBatteryInfo(HANDLE icdev, unsigned char tag, unsigned int *tlv_data_len, unsigned char *tlv_data);

  /**
   * @brief  推送二维码数据。
   * @par    说明：
   * 推送二维码数据。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_Push2DBarcodeData(HANDLE icdev);

  /**
   * @brief  射频用户属性操作（下电不保留状态）。
   * @par    说明：
   * 射频用户属性操作（下电不保留状态）。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类型。
   * @n 0x00 - 设置用户所有属性为设备缺省值，参数 @a value 无效。
   * @n 0x01 - 设置射频速率，参数 @a value 为入参，==0x00表示106K，==0x11表示212K，==0x33表示424K，==0x77表示848K。
   * @n 0x02 - 获取射频速率，参数 @a value 为出参，==0x00表示106K，==0x11表示212K，==0x33表示424K，==0x77表示848K。
   * @n 0x03 - 设置射频通讯请求WTX次数，参数 @a value 为入参。
   * @n 0x04 - 获取射频通讯请求WTX次数，参数 @a value 为出参。
   * @param[in,out] value 参数含义由 @a type 的值来决定。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_RfUserAttributes(HANDLE icdev, unsigned char type, unsigned short *value);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_OperateFirmwareVersionInfo(HANDLE icdev, unsigned char type, char *info);

  /**
   * @brief  启动功能按键操作。
   * @par    说明：
   * 启动功能按键操作。
   * @param[in] icdev 设备标识符。
   * @param[in] reserved 保留，固定为0x00。
   * @param[in] mode 控制模式。
   * @n bit0 - 表示是否需要按键当前按压状态，0表示不需要，1表示需要。
   * @n bit1 - 表示是否需要按键敲击次数，0表示不需要，1表示需要。
   * @n bit2~bit7 - 保留。
   * @param[in] data_len 数据的长度。
   * @param[in] data 数据，此数据每个字节为一个按键号，表示将要获取此按键号的相应信息，按键号取值范围为0x01~0xFE，值0x00和0xFF用作以下特殊用途。
   * @n 0x00 - 保留。
   * @n 0xFF - 表示将要获取全部按键的信息，如果使用0xFF，请不去使用0x01~0xFE，也就是数据部分仅为一个字节（即0xFF）。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_StartPushButton(HANDLE icdev, unsigned char reserved, unsigned char mode, unsigned int data_len, const unsigned char *data);

  /**
   * @brief  获取功能按键信息。
   * @par    说明：
   * 获取功能按键信息。
   * @param[in] icdev 设备标识符。
   * @param[out] info_len 按键信息的长度，此长度与 ::dc_StartPushButton 的 @a mode 无关，仅与它的 @a data 相关。
   * @param[out] info 按键信息。格式为：【1字节按键号】【1字节状态】【1字节次数】【1字节按键号】【1字节状态】【1字节次数】...。其中状态描述如下。
   * @n bit0 - 按键当前按压状态，0表示松开，1表示按下。
   * @n bit1~bit7 - 保留。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetPushButtonInfo(HANDLE icdev, unsigned int *info_len, unsigned char *info);

  /**
   * @brief  结束功能按键操作。
   * @par    说明：
   * 结束功能按键操作。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_EndPushButton(HANDLE icdev);

  /**
   * @brief  指示灯亮度控制。
   * @par    说明：
   * 指示灯亮度控制。
   * @param[in] icdev 设备标识符。
   * @param[in] number 指示灯编号，0表示全部指示灯，1表示第一个指示灯，2表示第二个指示灯，以此类推。
   * @param[in] step 档位亮度控制，0xFF表示无效，0x01表示亮度档位一，0x02表示亮度档位二，以此类推。
   * @param[in] stepless 无极亮度控制，0xFF表示无效，无极亮度范围为0x00~0x64。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_LedBrightnessControl(HANDLE icdev, unsigned char number, unsigned char step, unsigned char stepless);

  /**
   * @brief  指示灯亮度保存。
   * @par    说明：
   * 把指示灯的亮度状态信息进行永久保存。
   * @param[in] icdev 设备标识符。
   * @param[in] number 指示灯编号，0表示全部指示灯，1表示第一个指示灯，2表示第二个指示灯，以此类推。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_LedBrightnessSave(HANDLE icdev, unsigned char number);

  /**
   * @brief  指示灯亮度读取。
   * @par    说明：
   * 读取被永久保存的指示灯亮度状态信息。
   * @param[in] icdev 设备标识符。
   * @param[in] number 指示灯编号，0表示全部指示灯，1表示第一个指示灯，2表示第二个指示灯，以此类推。
   * @param[out] info_len 信息的长度。
   * @param[out] info 信息。格式为：【1字节指示灯编号】【1字节档位亮度】【1字节无极亮度】【1字节指示灯编号】【1字节档位亮度】【1字节无极亮度】...。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_LedBrightnessRead(HANDLE icdev, unsigned char number, unsigned int *info_len, unsigned char *info);

  /**
   * @brief  模块输出输入控制。
   * @par    说明：
   * 模块输出输入控制。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 控制模式。
   * @n 0x01 - 输出控制。
   * @n 0x02 - 输入控制（实时性状态）。
   * @param[in] slen 发送数据的长度。
   * @param[in] sdata 发送数据。格式为：【1字节模块编号】【1字节控制状态】...。模块编号取值范围为0x01~0xFE，值0xFF表示全部编号。控制状态描述如下。
   * @n 当控制模式为输出控制时，控制状态为0x01表示打开，控制状态为0x02表示关闭。
   * @n 当控制模式为输入控制时，控制状态固定为0x00。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据。
   * @n 当控制模式为输出控制时，无效。
   * @n 当控制模式为输入控制时，格式为：【1字节模块编号】【1字节控制状态】...。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ModuleOutInControl(HANDLE icdev, unsigned char mode, unsigned int slen, const unsigned char *sdata, unsigned int *rlen, unsigned char *rdata);

  /**
   * @brief  设置模块参数。
   * @par    说明：
   * 设置模块参数，永久保存。
   * @param[in] icdev 设备标识符。
   * @param[in] number 模块编号。
   * @n 0x0102 - 电磁锁。
   * @param[in] sub_number 子模块编号，从1开始编号，0表示默认行为。
   * @param[in] slen 发送数据的长度。
   * @param[in] sdata 发送数据，由一个或多个TLV格式组成，T和L固定为1个字节，T取值范围为0x01~0xFE。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SetModuleParameters(HANDLE icdev, int number, int sub_number, unsigned int slen, const unsigned char *sdata);

  /**
   * @brief  获取模块参数。
   * @par    说明：
   * 获取模块参数。
   * @param[in] icdev 设备标识符。
   * @param[in] number 模块编号。
   * @n 0x0102 - 电磁锁。
   * @param[in] sub_number 子模块编号，从1开始编号，0表示默认行为。
   * @param[in] slen 发送数据的长度。
   * @param[in] sdata 发送数据，存放标签列表，每个标签占1个字节，标签取值范围为0x01~0xFE，值0x00和0xFF用作以下特殊用途。
   * @n 0x00 - 保留。
   * @n 0xFF - 表示将要获取全部标签的信息，如果使用0xFF，请不去使用0x01~0xFE，也就是数据部分仅为一个字节（即0xFF）。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据，由一个或多个TLV格式组成，T和L固定为1个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetModuleParameters(HANDLE icdev, int number, int sub_number, unsigned int slen, const unsigned char *sdata, unsigned int *rlen, unsigned char *rdata);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_ThoroughControl(HANDLE icdev, int value);

  /**
   * @brief  查询固有能力。
   * @par    说明：
   * 查询固件代码固定支持的能力。
   * @param[in] icdev 设备标识符。
   * @param[in] slen 发送数据的长度。
   * @param[in] sdata 发送数据，存放标签列表，每个标签占1个字节，标签取值范围为0x01~0xFE，值0x00和0xFF用作以下特殊用途。
   * @n 0x00 - 保留。
   * @n 0xFF - 表示将要获取全部标签的信息，如果使用0xFF，请不去使用0x01~0xFE，也就是数据部分仅为一个字节（即0xFF）。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据，由一个或多个TLV格式组成，T和L固定为1个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_QueryInherentCapabilities(HANDLE icdev, unsigned int slen, const unsigned char *sdata, unsigned int *rlen, unsigned char *rdata);

  /**
   * @brief  外部EEPROM清除操作。
   * @par    说明：
   * 外部EEPROM清除操作。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_CleanOutEeprom(HANDLE icdev);

  /**
   * @brief  外部EEPROM读取日志条目数。
   * @par    说明：
   * 外部EEPROM读取日志条目数。
   * @param[in] icdev 设备标识符。
   * @param[out] count 日志总条目数。
   * @param[out] size 每个条目所占的固定字节数，如果为0表示不定长。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ReadOutEepromLogNumber(HANDLE icdev, unsigned int *count, unsigned int *size);

  /**
   * @brief  设置时钟定时开关机。
   * @par    说明：
   * 设置时钟定时开关机。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类型，0x01表示开机，0x02表示关机。
   * @param[in] number 组号，范围为1~255。
   * @param[in] hour 时，范围为0~23。
   * @param[in] minute 分，范围为0~59。
   * @param[in] second 秒，范围为0~59。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SetRtcTimeOnOff(HANDLE icdev, unsigned char type, unsigned char number, unsigned char hour, unsigned char minute, unsigned char second);

  /**
   * @brief  获取定时开关机的时钟。
   * @par    说明：
   * 获取定时开关机的时钟。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类型，0x01表示开机，0x02表示关机。
   * @param[in] number 组号，范围为1~255。
   * @param[out] hour 时，范围为0~23。
   * @param[out] minute 分，范围为0~59。
   * @param[out] second 秒，范围为0~59。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetRtcTimeOnOff(HANDLE icdev, unsigned char type, unsigned char number, unsigned char *hour, unsigned char *minute, unsigned char *second);

  /**
   * @brief  清除定时开关机。
   * @par    说明：
   * 清除定时开关机。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类型，0x01表示开机，0x02表示关机。
   * @param[in] number 组号，范围为1~255。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ClearRtcTimeOnOff(HANDLE icdev, unsigned char type, unsigned char number);

  /**
   * @brief  外部模块状态检测。
   * @par    说明：
   * 外部模块状态检测。
   * @param[in] icdev 设备标识符。
   * @param[in] module_number 模块编号。
   * @param[out] state 状态，0x00表示模块正常，0x01表示模块不存在，0x02表示模块异常。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ModuleStateDetection(HANDLE icdev, unsigned short module_number, unsigned char *state);

  /**
   * @brief  修改Mifare Ultralight AES卡密码。
   * @par    说明：
   * 使用传入的密码来修改Mifare Ultralight AES卡密码。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 保留，固定为0xFF。
   * @param[in] arg 密钥的类型。
   * @n 0x00 - DataProtKey。
   * @n 0x01 - UIDRetrKey。
   * @n 0x02 - OriginalityKey。
   * @param[in] keylen 密码长度，固定为16。
   * @param[in] newkey 新密码，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_changekey_aes(HANDLE icdev, unsigned char flag, unsigned char arg, int keylen, const unsigned char *newkey);

  /**
   * @brief  修改Mifare Ultralight AES卡密码。
   * @par    说明：
   * ::dc_changekey_aes 的HEX形式接口，参数 @a newkey 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_changekey_aes_hex(HANDLE icdev, unsigned char flag, unsigned char arg, int keylen, const char *newkey);

  /**
   * @brief  快速读卡片多块数据。
   * @par    说明：
   * 支持读取Utralight系列和NTAG系列卡片的多块数据。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 保留，固定为0xFF。
   * @param[in] start_addr 读块的起始地址。
   * @param[in] end_addr 读块的结束地址。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_fast_read(HANDLE icdev, unsigned char flag, int start_addr, int end_addr, int *rlen, unsigned char *rdata);

  /**
   * @brief  快速读卡片多块数据。
   * @par    说明：
   * ::dc_fast_read 的HEX形式接口，参数 @a rdata 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_fast_read_hex(HANDLE icdev, unsigned char flag, int start_addr, int end_addr, int *rlen, char *rdata);

  /**
   * @brief  读卡片计数器。
   * @par    说明：
   * 支持读取Utralight系列和NTAG系列卡片的计数器（部分卡片支持）。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 保留，固定为0xFF。
   * @param[in] addr 计数器地址。
   * @param[out] value 返回的计数器值。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_read_cnt(HANDLE icdev, unsigned char flag, int addr, unsigned int *value);

  /**
   * @brief  卡片计数器增加值。
   * @par    说明：
   * 支持增加Utralight系列和NTAG系列卡片计数器的计数值（部分卡片支持）。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 保留，固定为0xFF。
   * @param[in] addr 计数器地址。
   * @param[in] value 增加的计数值。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_incr_cnt(HANDLE icdev, unsigned char flag, int addr, unsigned int value);

  /**
   * @brief  读卡片签名数据。
   * @par    说明：
   * 支持读取Utralight系列和NTAG ECC卡的签名数据。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 保留，固定为0xFF。
   * @param[in] addr 签名数据地址，当前固定为0。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_read_sig(HANDLE icdev, unsigned char flag, int addr, int *rlen, unsigned char *rdata);

  /**
   * @brief  读卡片签名数据。
   * @par    说明：
   * ::dc_read_sig 的HEX形式接口，参数 @a rdata 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_read_sig_hex(HANDLE icdev, unsigned char flag, int addr, int *rlen, char *rdata);

  /**
   * @brief  写卡片签名数据。
   * @par    说明：
   * 支持写Utralight AES卡的自定义签名数据。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 保留，固定为0xFF。
   * @param[in] addr 页地址。
   * @param[in] slen 写入数据的长度。
   * @param[in] sdata 写入数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_write_sig(HANDLE icdev, unsigned char flag, int addr, int slen, const unsigned char *sdata);

  /**
   * @brief  写卡片签名数据。
   * @par    说明：
   * ::dc_write_sig 的HEX形式接口，参数 @a sdata 为HEX格式。
   */
  API_ATTRIBUTE short USER_API dc_write_sig_hex(HANDLE icdev, unsigned char flag, int addr, int slen, const char *sdata);

  /**
   * @brief  卡片签名的锁定、解锁操作。
   * @par    说明：
   * Utralight AES卡的自定义签名数据解锁、锁定、永久锁定操作。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 保留，固定为0xFF。
   * @param[in] arg 操作参数。
   * @n 0x00 - 解锁。
   * @n 0x01 - 锁定。
   * @n 0x02 - 永久锁定。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_lock_sig(HANDLE icdev, unsigned char flag, unsigned char arg);

  /**
   * @brief  读卡片内部版本信息。
   * @par    说明：
   * 读取Mifare系列卡片的内部版本信息（部分卡片支持）。
   * @param[in] icdev 设备标识符。
   * @param[in] flag 保留，固定为0xFF。
   * @param[out] rlen 返回数据的长度。
   * @param[out] rdata 返回的数据。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_getcard_version(HANDLE icdev, unsigned char flag, int *rlen, unsigned char *rdata);

  /**
   * @brief  分析卡片内部版本信息。
   * @par    说明：
   * 分析卡片内部版本信息。
   * @param[in] flag 保留，固定为0xFF。
   * @param[in] index 需要解析的版本信息字节索引，范围为0~7，其中6可返回卡型。
   * @param[in] version_data_len 版本数据的长度。
   * @param[in] version_data 版本数据。
   * @param[out] version_exp 解析后的版本信息。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_analyze_version(unsigned char flag, unsigned char index, int version_data_len, const unsigned char *version_data, char *version_exp);

  /**
   * @brief  电机驱动控制。
   * @par    说明：
   * 电机驱动控制。
   * @param[in] icdev 设备标识符。
   * @param[in] mode 控制模式，0x01表示前进，0x02表示后退，0x03表示停止。
   * @return <0表示失败，==0表示成功，==1表示驱动失败，==2表示顶部限位停止运动，==3表示底部限位停止运动，==4表示温度过高停止运动。
   */
  API_ATTRIBUTE short USER_API dc_MotorDriveControl(HANDLE icdev, unsigned char mode);

  /**
   * @brief  设置电机位置。
   * @par    说明：
   * 设置电机位置。
   * @param[in] icdev 设备标识符。
   * @param[in] value 电机位置地址值，绝对地址位置，最高位置可以获取电机参数得到。
   * @return <0表示失败，==0表示成功，==1表示驱动失败，==2表示温度过高停止运动。
   */
  API_ATTRIBUTE short USER_API dc_MotorDriveSetPosition(HANDLE icdev, unsigned short value);

  /**
   * @brief  设置电机速度。
   * @par    说明：
   * 设置电机速度。
   * @param[in] icdev 设备标识符。
   * @param[in] value 电机速度值（0x01~0x05）。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_MotorDriveSetSpeed(HANDLE icdev, unsigned char value);

  /**
   * @brief  复位电机位置。
   * @par    说明：
   * 复位电机位置。
   * @param[in] icdev 设备标识符。
   * @param[in] value 电机位置标记值，0x01表示底部位置，0x02表示中间位置，0x03表示顶部位置。
   * @return <0表示失败，==0表示成功，==1表示驱动失败，==2表示温度过高停止运动。
   */
  API_ATTRIBUTE short USER_API dc_MotorDriveResetPosition(HANDLE icdev, unsigned char value);

  /**
   * @brief  获取电机参数。
   * @par    说明：
   * 获取电机参数。
   * @param[in] icdev 设备标识符。
   * @param[out] info_len 返回参数信息的长度。
   * @param[out] info_data 返回的参数信息，由多个TLV格式组成，Tag占用1个字节，Len占用1个字节，下面是Tag说明。
   * @n 0x01 - 电推杆实际物理总高度，单位为mm。
   * @n 0x02 - 电推杆绝对位置高度。
   * @n 0x03 - 电机当前所在位置高度。
   * @n 0x04 - 电机驱动速度。
   * @n 0x05 - 顶部传感器状态，0x00表示无感应，0x01表示有感应。
   * @n 0x06 - 底部传感器状态，0x00表示无感应，0x01表示有感应。
   * @n 0x07 - 电机温度传感器状态，0x00表示正常，0x01表示高温。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_MotorDriveGetParameters(HANDLE icdev, int *info_len, unsigned char *info_data);

  /**
   * @brief  获取社保卡基本信息。
   * @par    说明：
   * 获取社保卡基本信息。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类型。
   * @n byte0 - 用户卡类别，0x00表示非接触式CPU卡，0x01表示接触式CPU卡。
   * @n byte1 - PSAM卡卡座，0x00表示SAM1卡座，0x01表示SAM2卡座，以此类推。
   * @n byte2~byte7 - 保留，必须为0x00。
   * @param[out] card_code 卡的识别码，请至少分配33个字节。
   * @param[out] card_type 卡的类别，请至少分配2个字节。
   * @param[out] version 规范版本，请至少分配5个字节。
   * @param[out] init_org_number 初始化机构编号，请至少分配25个字节。
   * @param[out] card_issue_date 发卡日期，请至少分配9个字节。
   * @param[out] card_expire_day 卡有效期，请至少分配9个字节。
   * @param[out] card_number 卡号，请至少分配10个字节。
   * @param[out] social_security_number 社会保障号码，请至少分配19个字节。
   * @param[out] name 姓名，请至少分配31个字节。
   * @param[out] name_ex 姓名扩展，请至少分配21个字节。
   * @param[out] sex 性别，请至少分配2个字节。
   * @param[out] nation 民族，请至少分配3个字节。
   * @param[out] birth_place 出生地，请至少分配7个字节。
   * @param[out] birth_day 出生日期，请至少分配9个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetSocialSecurityCardBaseInfo(HANDLE icdev, int type, char *card_code, char *card_type, char *version, char *init_org_number, char *card_issue_date, char *card_expire_day, char *card_number, char *social_security_number, char *name, char *name_ex, char *sex, char *nation, char *birth_place, char *birth_day);

  /**
   * @brief  修改社保卡的卡密码。
   * @par    说明：
   * 修改社保卡的卡密码。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类型，0表示非接触式，1表示接触式。
   * @param[in] old_password 旧密码。
   * @param[in] new_password 新密码。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_ChangeSocialSecurityCardPassword(HANDLE icdev, int type, const char *old_password, const char *new_password);

  /**
   * @brief  获取银行卡帐户号码。
   * @par    说明：
   * 获取银行卡帐户号码。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类型，0表示非接触式，1表示接触式。
   * @param[out] number 返回的银行卡帐户号码字符串，请至少分配64个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetBankAccountNumber(HANDLE icdev, int type, char *number);

  /**
   * @brief  获取银行卡磁条信息。
   * @par    说明：
   * 获取银行卡磁条信息。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类型，0表示非接触式，1表示接触式。
   * @param[out] track1 返回的银行卡磁条轨道1信息字符串，请至少分配512个字节。
   * @param[out] track2 返回的银行卡磁条轨道2信息字符串，请至少分配512个字节。
   * @param[out] track3 返回的银行卡磁条轨道3信息字符串，请至少分配512个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_GetBankMagInfo(HANDLE icdev, int type, char *track1, char *track2, char *track3);

  /**
   * @brief  生成身份证正面和背面图像。
   * @par    说明：
   * 生成身份证正面和背面图像。
   * @param[in] icdev 设备标识符。
   * @param[in] type 类型，0表示中国人居民身份证，1表示外国人永久居留证，2表示港澳台居民居住证。
   * @param[in] text_len 文字信息的长度。
   * @param[in] text 文字信息。
   * @param[in] photo_len 相片信息的长度。
   * @param[in] photo 相片信息。
   * @param[in] front_file 传入正面图像文件名，请确保有写入的权限。
   * @param[in] back_file 传入背面图像文件名，请确保有写入的权限。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_IdCardImageBuild(HANDLE icdev, int type, int text_len, const unsigned char *text, int photo_len, const unsigned char *photo, const char *front_file, const char *back_file);

  /**
   * @brief  创建文件结构操作。
   * @par    说明：
   * 创建文件结构操作。
   * @param[in] icdev 设备标识符。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SAM_CreateFileSystem(HANDLE icdev);

  /**
   * @brief  写SM4加密密钥操作。
   * @par    说明：
   * 写SM4加密密钥操作。
   * @param[in] icdev 设备标识符。
   * @param[in] pucKey 密钥值，固定为16个字节。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SAM_WriteKeySM4(HANDLE icdev, const unsigned char *pucKey);

  /**
   * @brief  SM4加密操作。
   * @par    说明：
   * SM4加密操作。
   * @param[in] icdev 设备标识符。
   * @param[in] pucIV 初始值，固定为16个字节。
   * @param[in] pucInput 输入加密数据。
   * @param[in] iInputLen 输入加密数据的长度。
   * @param[out] pucOutput 返回加密结果。
   * @param[out] piOutputLen 返回加密结果的长度。
   * @return <0表示失败，==0表示成功。
   */
  API_ATTRIBUTE short USER_API dc_SAM_AlgSM4EncryptECB(HANDLE icdev, const unsigned char *pucIV, const unsigned char *pucInput, int iInputLen, unsigned char *pucOutput, int *piOutputLen);

  /**
   * @brief  保留。
   */
  API_ATTRIBUTE short USER_API dc_DeviceSelfChecking(const char *name, const char *port_type, const char *port_attributes, int code, const unsigned char *in_data, int in_len, unsigned char *out_data, int *out_len);

#ifdef __cplusplus
}
#endif

#ifdef WIN32
#pragma pack(pop)
#endif

#endif // DCRF32_H_
