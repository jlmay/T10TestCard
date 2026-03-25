#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#ifndef WIN32
#include <sys/time.h>
#endif
#include "dcrf32.h"

#if (__cplusplus < 201103L) && !(defined(_MSC_VER) && _MSC_VER >= 1910)
#define nullptr 0
#endif

static int ObtainFileData(const char *name, unsigned char **data) {
  FILE *fp;
  unsigned char *file_data;
  int file_size, x, y, z;

  *data = nullptr;

  fp = fopen(name, "rb");
  if (fp == nullptr) {
    return -1;
  }
  fseek(fp, 0, SEEK_END);
  file_size = (int)ftell(fp);
  file_data = (unsigned char *)malloc(file_size);
  if (file_data == nullptr) {
    fclose(fp);
    return -1;
  }
  fseek(fp, 0, SEEK_SET);
  x = file_size;
  y = 0;
  while (x > 0) {
    z = (int)fread(&file_data[y], 1, x, fp);
    if (z <= 0) {
      free(file_data);
      fclose(fp);
      return -1;
    }
    y += z;
    x -= z;
  }
  fclose(fp);

  *data = file_data;

  return file_size;
}

#ifdef WIN32
typedef __int64 BigInt;
static BigInt GetCurrentTimeTick() {
  return GetTickCount();
}
#else
typedef long long BigInt;
static BigInt GetCurrentTimeTick() {
  timeval tv;
  gettimeofday(&tv, nullptr);
  return ((BigInt)tv.tv_sec) * 1000 + tv.tv_usec / 1000;
}
#endif

static void Random(int len, unsigned char *data) {
  int i;

  srand((unsigned int)GetCurrentTimeTick());
  for (i = 0; i < len; ++i) {
    data[i] = rand();
  }
}

static const unsigned char *ConvertString(const unsigned char *in_str, unsigned char *out_str) {
#ifdef WIN32
  return in_str;
#else
  dc_string_converter(2, in_str, out_str);
  return out_str;
#endif
}

static const unsigned char *ConvertString(int type, const unsigned char *in_str, unsigned char *out_str) {
  dc_string_converter(type, in_str, out_str);
  return out_str;
}

static int IdCardTest(HANDLE handle) {
  int result;
  unsigned char info_buffer[64];
  int text_len;
  unsigned char text[256];
  int photo_len;
  unsigned char photo[1024];
  int fingerprint_len;
  unsigned char fingerprint[1024];
  int extra_len;
  unsigned char extra[70];
  int type;
#ifndef WIN32
  unsigned char title[128];
#endif

  printf("\n-------- ID Card Test --------\n");

  printf("dc_SamAReadCardInfo ... ");
  result = dc_SamAReadCardInfo(handle, 3, &text_len, text, &photo_len, photo, &fingerprint_len, fingerprint, &extra_len, extra);
  if (result != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_GetIdCardType ... ");
  type = dc_GetIdCardType(handle, text_len, text);
  if (type < 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  if (type == 0) {
    unsigned char name[64];
    unsigned char sex[8];
    unsigned char nation[12];
    unsigned char birth_day[36];
    unsigned char address[144];
    unsigned char id_number[76];
    unsigned char department[64];
    unsigned char expire_start_day[36];
    unsigned char expire_end_day[36];
    unsigned char reserved[76];

    printf("dc_ParseTextInfo ... ");
#ifdef WIN32
    result = dc_ParseTextInfo(handle, 1, text_len, text, name, sex, nation, birth_day, address, id_number, department, expire_start_day, expire_end_day, reserved);
#else
    result = dc_ParseTextInfo(handle, 2, text_len, text, name, sex, nation, birth_day, address, id_number, department, expire_start_day, expire_end_day, reserved);
#endif
    if (result != 0) {
      printf("error!\n");
      return -1;
    }
    printf("ok!\n");
#ifdef WIN32
    wprintf(L"%s: %s\n", L"姓名", (wchar_t *)name);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"姓名", title), name);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x10, sex, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x20, sex, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s\n", L"性别", (wchar_t *)info_buffer);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"性别", title), info_buffer);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x11, nation, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x21, nation, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s\n", L"民族", (wchar_t *)info_buffer);
    wprintf(L"%s: %s\n", L"出生日期", (wchar_t *)birth_day);
    wprintf(L"%s: %s\n", L"住址", (wchar_t *)address);
    wprintf(L"%s: %s\n", L"公民身份号码", (wchar_t *)id_number);
    wprintf(L"%s: %s\n", L"签发机关", (wchar_t *)department);
    wprintf(L"%s: %s\n", L"证件签发日期", (wchar_t *)expire_start_day);
    wprintf(L"%s: %s\n", L"证件终止日期", (wchar_t *)expire_end_day);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"民族", title), info_buffer);
    printf("%s: %s\n", ConvertString((unsigned char *)"出生日期", title), birth_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"住址", title), address);
    printf("%s: %s\n", ConvertString((unsigned char *)"公民身份号码", title), id_number);
    printf("%s: %s\n", ConvertString((unsigned char *)"签发机关", title), department);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件签发日期", title), expire_start_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件终止日期", title), expire_end_day);
#endif
  } else if (type == 1) {
    unsigned char english_name[244];
    unsigned char sex[8];
    unsigned char id_number[64];
    unsigned char citizenship[16];
    unsigned char chinese_name[64];
    unsigned char expire_start_day[36];
    unsigned char expire_end_day[36];
    unsigned char birth_day[36];
    unsigned char version_number[12];
    unsigned char department_code[20];
    unsigned char type_sign[8];
    unsigned char reserved[16];

    printf("dc_ParseTextInfoForForeigner ... ");
#ifdef WIN32
    result = dc_ParseTextInfoForForeigner(handle, 1, text_len, text, english_name, sex, id_number, citizenship, chinese_name, expire_start_day, expire_end_day, birth_day, version_number, department_code, type_sign, reserved);
#else
    result = dc_ParseTextInfoForForeigner(handle, 2, text_len, text, english_name, sex, id_number, citizenship, chinese_name, expire_start_day, expire_end_day, birth_day, version_number, department_code, type_sign, reserved);
#endif
    if (result != 0) {
      printf("error!\n");
      return -1;
    }
    printf("ok!\n");
#ifdef WIN32
    wprintf(L"%s: %s\n", L"英文姓名", (wchar_t *)english_name);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"英文姓名", title), english_name);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x10, sex, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x20, sex, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s/%s\n", L"性别", (wchar_t *)info_buffer, (wcscmp((wchar_t *)sex, L"1") == 0) ? L"M" : L"F");
    wprintf(L"%s: %s\n", L"永久居留证号码", (wchar_t *)id_number);
#else
    printf("%s: %s/%s\n", ConvertString((unsigned char *)"性别", title), info_buffer, (strcmp((char *)sex, "1") == 0) ? "M" : "F");
    printf("%s: %s\n", ConvertString((unsigned char *)"永久居留证号码", title), id_number);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x12, citizenship, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x22, citizenship, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s/%s\n", L"国籍或所在地区代码", (wchar_t *)info_buffer, (wchar_t *)citizenship);
    wprintf(L"%s: %s\n", L"中文姓名", (wchar_t *)chinese_name);
    wprintf(L"%s: %s\n", L"证件签发日期", (wchar_t *)expire_start_day);
    wprintf(L"%s: %s\n", L"证件终止日期", (wchar_t *)expire_end_day);
    wprintf(L"%s: %s\n", L"出生日期", (wchar_t *)birth_day);
    wprintf(L"%s: %s\n", L"证件版本号", (wchar_t *)version_number);
    wprintf(L"%s: %s\n", L"当次申请受理机关代码", (wchar_t *)department_code);
    wprintf(L"%s: %s\n", L"证件类型标识", (wchar_t *)type_sign);
#else
    printf("%s: %s/%s\n", ConvertString((unsigned char *)"国籍或所在地区代码", title), info_buffer, citizenship);
    printf("%s: %s\n", ConvertString((unsigned char *)"中文姓名", title), chinese_name);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件签发日期", title), expire_start_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件终止日期", title), expire_end_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"出生日期", title), birth_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件版本号", title), version_number);
    printf("%s: %s\n", ConvertString((unsigned char *)"当次申请受理机关代码", title), department_code);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件类型标识", title), type_sign);
#endif
  } else if (type == 2) {
    unsigned char name[64];
    unsigned char sex[8];
    unsigned char reserved[12];
    unsigned char birth_day[36];
    unsigned char address[144];
    unsigned char id_number[76];
    unsigned char department[64];
    unsigned char expire_start_day[36];
    unsigned char expire_end_day[36];
    unsigned char pass_number[40];
    unsigned char sign_count[12];
    unsigned char reserved2[16];
    unsigned char type_sign[8];
    unsigned char reserved3[16];

    printf("dc_ParseTextInfoForHkMoTw ... ");
#ifdef WIN32
    result = dc_ParseTextInfoForHkMoTw(handle, 1, text_len, text, name, sex, reserved, birth_day, address, id_number, department, expire_start_day, expire_end_day, pass_number, sign_count, reserved2, type_sign, reserved3);
#else
    result = dc_ParseTextInfoForHkMoTw(handle, 2, text_len, text, name, sex, reserved, birth_day, address, id_number, department, expire_start_day, expire_end_day, pass_number, sign_count, reserved2, type_sign, reserved3);
#endif
    if (result != 0) {
      printf("error!\n");
      return -1;
    }
    printf("ok!\n");
#ifdef WIN32
    wprintf(L"%s: %s\n", L"姓名", (wchar_t *)name);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"姓名", title), name);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x10, sex, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x20, sex, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s\n", L"性别", (wchar_t *)info_buffer);
    wprintf(L"%s: %s\n", L"出生日期", (wchar_t *)birth_day);
    wprintf(L"%s: %s\n", L"住址", (wchar_t *)address);
    wprintf(L"%s: %s\n", L"公民身份号码", (wchar_t *)id_number);
    wprintf(L"%s: %s\n", L"签发机关", (wchar_t *)department);
    wprintf(L"%s: %s\n", L"证件签发日期", (wchar_t *)expire_start_day);
    wprintf(L"%s: %s\n", L"证件终止日期", (wchar_t *)expire_end_day);
    wprintf(L"%s: %s\n", L"通行证号码", (wchar_t *)pass_number);
    wprintf(L"%s: %s\n", L"签发次数", (wchar_t *)sign_count);
    wprintf(L"%s: %s\n", L"证件类型标识", (wchar_t *)type_sign);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"性别", title), info_buffer);
    printf("%s: %s\n", ConvertString((unsigned char *)"出生日期", title), birth_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"住址", title), address);
    printf("%s: %s\n", ConvertString((unsigned char *)"公民身份号码", title), id_number);
    printf("%s: %s\n", ConvertString((unsigned char *)"签发机关", title), department);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件签发日期", title), expire_start_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件终止日期", title), expire_end_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"通行证号码", title), pass_number);
    printf("%s: %s\n", ConvertString((unsigned char *)"签发次数", title), sign_count);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件类型标识", title), type_sign);
#endif
  } else if (type == 3) {
    unsigned char chinese_name[64];
    unsigned char sex[8];
    unsigned char renew_count[12];
    unsigned char birth_day[36];
    unsigned char english_name[144];
    unsigned char id_number[76];
    unsigned char reserved[64];
    unsigned char expire_start_day[36];
    unsigned char expire_end_day[36];
    unsigned char english_name_ex[48];
    unsigned char citizenship[16];
    unsigned char type_sign[8];
    unsigned char prev_related_info[16];
    unsigned char old_id_number[64];

    printf("dc_ParseTextInfoForNewForeigner ... ");
#ifdef WIN32
    result = dc_ParseTextInfoForNewForeigner(handle, 1, text_len, text, chinese_name, sex, renew_count, birth_day, english_name, id_number, reserved, expire_start_day, expire_end_day, english_name_ex, citizenship, type_sign, prev_related_info, old_id_number);
#else
    result = dc_ParseTextInfoForNewForeigner(handle, 2, text_len, text, chinese_name, sex, renew_count, birth_day, english_name, id_number, reserved, expire_start_day, expire_end_day, english_name_ex, citizenship, type_sign, prev_related_info, old_id_number);
#endif
    if (result != 0) {
      printf("error!\n");
      return -1;
    }
    printf("ok!\n");
#ifdef WIN32
    wprintf(L"%s: %s\n", L"中文姓名", (wchar_t *)chinese_name);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"中文姓名", title), chinese_name);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x10, sex, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x20, sex, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s/%s\n", L"性别", (wchar_t *)info_buffer, (wcscmp((wchar_t *)sex, L"1") == 0) ? L"M" : L"F");
    wprintf(L"%s: %s\n", L"换证次数", (wchar_t *)renew_count);
    wprintf(L"%s: %s\n", L"出生日期", (wchar_t *)birth_day);
    wprintf(L"%s: %s%s\n", L"英文姓名", (wchar_t *)english_name, (wchar_t *)english_name_ex);
    wprintf(L"%s: %s\n", L"证件号码", (wchar_t *)id_number);
    wprintf(L"%s: %s\n", L"有效期起始日期", (wchar_t *)expire_start_day);
    wprintf(L"%s: %s\n", L"有效期截止日期", (wchar_t *)expire_end_day);
#else
    printf("%s: %s/%s\n", ConvertString((unsigned char *)"性别", title), info_buffer, (strcmp((char *)sex, "1") == 0) ? "M" : "F");
    printf("%s: %s\n", ConvertString((unsigned char *)"换证次数", title), renew_count);
    printf("%s: %s\n", ConvertString((unsigned char *)"出生日期", title), birth_day);
    printf("%s: %s%s\n", ConvertString((unsigned char *)"英文姓名", title), english_name, english_name_ex);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件号码", title), id_number);
    printf("%s: %s\n", ConvertString((unsigned char *)"有效期起始日期", title), expire_start_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"有效期截止日期", title), expire_end_day);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x12, citizenship, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x22, citizenship, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s/%s\n", L"国籍", (wchar_t *)info_buffer, (wchar_t *)citizenship);
    wprintf(L"%s: %s\n", L"证件类型标识", (wchar_t *)type_sign);
    wprintf(L"%s: %s\n", L"既往版本外国人永久居留证件号码关联项", (wchar_t *)prev_related_info);
    if (wcslen((wchar_t *)prev_related_info) != 0) {
      wprintf(L"%s%s%s\n", L"持证人曾持有号码为", (wchar_t *)old_id_number, L"的外国人永久居留身份证");
    }
#else
    printf("%s: %s/%s\n", ConvertString((unsigned char *)"国籍", title), info_buffer, citizenship);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件类型标识", title), type_sign);
    printf("%s: %s\n", ConvertString((unsigned char *)"既往版本外国人永久居留证件号码关联项", title), prev_related_info);
    if (strlen((char *)prev_related_info) != 0) {
      printf("%s%s%s\n", ConvertString((unsigned char *)"持证人曾持有号码为", title), old_id_number, ConvertString((unsigned char *)"的外国人永久居留身份证", title));
    }
#endif
  }

  printf("dc_ParsePhotoInfo ... ");
  result = dc_ParsePhotoInfo(handle, 0, photo_len, photo, 0, (unsigned char *)"me.bmp");
  if (result != 0) {
    if (result == -2) {
      printf("failed to call photo decoding library!\n");
    } else {
      printf("error!\n");
    }
    return -1;
  }
  printf("ok!\n");

  return 0;
}

static int IdCardTest2(HANDLE handle) {
  int result;
  unsigned char info_buffer[64];
  int text_len;
  unsigned char text[256];
  int photo_len;
  unsigned char photo[1024];
  int fingerprint_len;
  unsigned char fingerprint[1024];
  int extra_len;
  unsigned char extra[70];
  int type;
#ifndef WIN32
  unsigned char title[128];
#endif

  printf("\n-------- ID Card Test (Hardware Acceleration) --------\n");

  printf("dc_IdCardReadCardInfo ... ");
  result = dc_IdCardReadCardInfo(handle, 0x02, 0x0F, &text_len, text, &photo_len, photo, &fingerprint_len, fingerprint, &extra_len, extra);
  if (result != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_GetIdCardType ... ");
  type = dc_GetIdCardType(handle, text_len, text);
  if (type < 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  if (type == 0) {
    unsigned char name[64];
    unsigned char sex[8];
    unsigned char nation[12];
    unsigned char birth_day[36];
    unsigned char address[144];
    unsigned char id_number[76];
    unsigned char department[64];
    unsigned char expire_start_day[36];
    unsigned char expire_end_day[36];
    unsigned char reserved[76];

    printf("dc_ParseTextInfo ... ");
#ifdef WIN32
    result = dc_ParseTextInfo(handle, 1, text_len, text, name, sex, nation, birth_day, address, id_number, department, expire_start_day, expire_end_day, reserved);
#else
    result = dc_ParseTextInfo(handle, 2, text_len, text, name, sex, nation, birth_day, address, id_number, department, expire_start_day, expire_end_day, reserved);
#endif
    if (result != 0) {
      printf("error!\n");
      return -1;
    }
    printf("ok!\n");
#ifdef WIN32
    wprintf(L"%s: %s\n", L"姓名", (wchar_t *)name);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"姓名", title), name);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x10, sex, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x20, sex, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s\n", L"性别", (wchar_t *)info_buffer);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"性别", title), info_buffer);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x11, nation, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x21, nation, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s\n", L"民族", (wchar_t *)info_buffer);
    wprintf(L"%s: %s\n", L"出生日期", (wchar_t *)birth_day);
    wprintf(L"%s: %s\n", L"住址", (wchar_t *)address);
    wprintf(L"%s: %s\n", L"公民身份号码", (wchar_t *)id_number);
    wprintf(L"%s: %s\n", L"签发机关", (wchar_t *)department);
    wprintf(L"%s: %s\n", L"证件签发日期", (wchar_t *)expire_start_day);
    wprintf(L"%s: %s\n", L"证件终止日期", (wchar_t *)expire_end_day);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"民族", title), info_buffer);
    printf("%s: %s\n", ConvertString((unsigned char *)"出生日期", title), birth_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"住址", title), address);
    printf("%s: %s\n", ConvertString((unsigned char *)"公民身份号码", title), id_number);
    printf("%s: %s\n", ConvertString((unsigned char *)"签发机关", title), department);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件签发日期", title), expire_start_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件终止日期", title), expire_end_day);
#endif
  } else if (type == 1) {
    unsigned char english_name[244];
    unsigned char sex[8];
    unsigned char id_number[64];
    unsigned char citizenship[16];
    unsigned char chinese_name[64];
    unsigned char expire_start_day[36];
    unsigned char expire_end_day[36];
    unsigned char birth_day[36];
    unsigned char version_number[12];
    unsigned char department_code[20];
    unsigned char type_sign[8];
    unsigned char reserved[16];

    printf("dc_ParseTextInfoForForeigner ... ");
#ifdef WIN32
    result = dc_ParseTextInfoForForeigner(handle, 1, text_len, text, english_name, sex, id_number, citizenship, chinese_name, expire_start_day, expire_end_day, birth_day, version_number, department_code, type_sign, reserved);
#else
    result = dc_ParseTextInfoForForeigner(handle, 2, text_len, text, english_name, sex, id_number, citizenship, chinese_name, expire_start_day, expire_end_day, birth_day, version_number, department_code, type_sign, reserved);
#endif
    if (result != 0) {
      printf("error!\n");
      return -1;
    }
    printf("ok!\n");
#ifdef WIN32
    wprintf(L"%s: %s\n", L"英文姓名", (wchar_t *)english_name);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"英文姓名", title), english_name);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x10, sex, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x20, sex, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s/%s\n", L"性别", (wchar_t *)info_buffer, (wcscmp((wchar_t *)sex, L"1") == 0) ? L"M" : L"F");
    wprintf(L"%s: %s\n", L"永久居留证号码", (wchar_t *)id_number);
#else
    printf("%s: %s/%s\n", ConvertString((unsigned char *)"性别", title), info_buffer, (strcmp((char *)sex, "1") == 0) ? "M" : "F");
    printf("%s: %s\n", ConvertString((unsigned char *)"永久居留证号码", title), id_number);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x12, citizenship, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x22, citizenship, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s/%s\n", L"国籍或所在地区代码", (wchar_t *)info_buffer, (wchar_t *)citizenship);
    wprintf(L"%s: %s\n", L"中文姓名", (wchar_t *)chinese_name);
    wprintf(L"%s: %s\n", L"证件签发日期", (wchar_t *)expire_start_day);
    wprintf(L"%s: %s\n", L"证件终止日期", (wchar_t *)expire_end_day);
    wprintf(L"%s: %s\n", L"出生日期", (wchar_t *)birth_day);
    wprintf(L"%s: %s\n", L"证件版本号", (wchar_t *)version_number);
    wprintf(L"%s: %s\n", L"当次申请受理机关代码", (wchar_t *)department_code);
    wprintf(L"%s: %s\n", L"证件类型标识", (wchar_t *)type_sign);
#else
    printf("%s: %s/%s\n", ConvertString((unsigned char *)"国籍或所在地区代码", title), info_buffer, citizenship);
    printf("%s: %s\n", ConvertString((unsigned char *)"中文姓名", title), chinese_name);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件签发日期", title), expire_start_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件终止日期", title), expire_end_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"出生日期", title), birth_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件版本号", title), version_number);
    printf("%s: %s\n", ConvertString((unsigned char *)"当次申请受理机关代码", title), department_code);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件类型标识", title), type_sign);
#endif
  } else if (type == 2) {
    unsigned char name[64];
    unsigned char sex[8];
    unsigned char reserved[12];
    unsigned char birth_day[36];
    unsigned char address[144];
    unsigned char id_number[76];
    unsigned char department[64];
    unsigned char expire_start_day[36];
    unsigned char expire_end_day[36];
    unsigned char pass_number[40];
    unsigned char sign_count[12];
    unsigned char reserved2[16];
    unsigned char type_sign[8];
    unsigned char reserved3[16];

    printf("dc_ParseTextInfoForHkMoTw ... ");
#ifdef WIN32
    result = dc_ParseTextInfoForHkMoTw(handle, 1, text_len, text, name, sex, reserved, birth_day, address, id_number, department, expire_start_day, expire_end_day, pass_number, sign_count, reserved2, type_sign, reserved3);
#else
    result = dc_ParseTextInfoForHkMoTw(handle, 2, text_len, text, name, sex, reserved, birth_day, address, id_number, department, expire_start_day, expire_end_day, pass_number, sign_count, reserved2, type_sign, reserved3);
#endif
    if (result != 0) {
      printf("error!\n");
      return -1;
    }
    printf("ok!\n");
#ifdef WIN32
    wprintf(L"%s: %s\n", L"姓名", (wchar_t *)name);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"姓名", title), name);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x10, sex, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x20, sex, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s\n", L"性别", (wchar_t *)info_buffer);
    wprintf(L"%s: %s\n", L"出生日期", (wchar_t *)birth_day);
    wprintf(L"%s: %s\n", L"住址", (wchar_t *)address);
    wprintf(L"%s: %s\n", L"公民身份号码", (wchar_t *)id_number);
    wprintf(L"%s: %s\n", L"签发机关", (wchar_t *)department);
    wprintf(L"%s: %s\n", L"证件签发日期", (wchar_t *)expire_start_day);
    wprintf(L"%s: %s\n", L"证件终止日期", (wchar_t *)expire_end_day);
    wprintf(L"%s: %s\n", L"通行证号码", (wchar_t *)pass_number);
    wprintf(L"%s: %s\n", L"签发次数", (wchar_t *)sign_count);
    wprintf(L"%s: %s\n", L"证件类型标识", (wchar_t *)type_sign);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"性别", title), info_buffer);
    printf("%s: %s\n", ConvertString((unsigned char *)"出生日期", title), birth_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"住址", title), address);
    printf("%s: %s\n", ConvertString((unsigned char *)"公民身份号码", title), id_number);
    printf("%s: %s\n", ConvertString((unsigned char *)"签发机关", title), department);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件签发日期", title), expire_start_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件终止日期", title), expire_end_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"通行证号码", title), pass_number);
    printf("%s: %s\n", ConvertString((unsigned char *)"签发次数", title), sign_count);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件类型标识", title), type_sign);
#endif
  } else if (type == 3) {
    unsigned char chinese_name[64];
    unsigned char sex[8];
    unsigned char renew_count[12];
    unsigned char birth_day[36];
    unsigned char english_name[144];
    unsigned char id_number[76];
    unsigned char reserved[64];
    unsigned char expire_start_day[36];
    unsigned char expire_end_day[36];
    unsigned char english_name_ex[48];
    unsigned char citizenship[16];
    unsigned char type_sign[8];
    unsigned char prev_related_info[16];
    unsigned char old_id_number[64];

    printf("dc_ParseTextInfoForNewForeigner ... ");
#ifdef WIN32
    result = dc_ParseTextInfoForNewForeigner(handle, 1, text_len, text, chinese_name, sex, renew_count, birth_day, english_name, id_number, reserved, expire_start_day, expire_end_day, english_name_ex, citizenship, type_sign, prev_related_info, old_id_number);
#else
    result = dc_ParseTextInfoForNewForeigner(handle, 2, text_len, text, chinese_name, sex, renew_count, birth_day, english_name, id_number, reserved, expire_start_day, expire_end_day, english_name_ex, citizenship, type_sign, prev_related_info, old_id_number);
#endif
    if (result != 0) {
      printf("error!\n");
      return -1;
    }
    printf("ok!\n");
#ifdef WIN32
    wprintf(L"%s: %s\n", L"中文姓名", (wchar_t *)chinese_name);
#else
    printf("%s: %s\n", ConvertString((unsigned char *)"中文姓名", title), chinese_name);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x10, sex, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x20, sex, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s/%s\n", L"性别", (wchar_t *)info_buffer, (wcscmp((wchar_t *)sex, L"1") == 0) ? L"M" : L"F");
    wprintf(L"%s: %s\n", L"换证次数", (wchar_t *)renew_count);
    wprintf(L"%s: %s\n", L"出生日期", (wchar_t *)birth_day);
    wprintf(L"%s: %s%s\n", L"英文姓名", (wchar_t *)english_name, (wchar_t *)english_name_ex);
    wprintf(L"%s: %s\n", L"证件号码", (wchar_t *)id_number);
    wprintf(L"%s: %s\n", L"有效期起始日期", (wchar_t *)expire_start_day);
    wprintf(L"%s: %s\n", L"有效期截止日期", (wchar_t *)expire_end_day);
#else
    printf("%s: %s/%s\n", ConvertString((unsigned char *)"性别", title), info_buffer, (strcmp((char *)sex, "1") == 0) ? "M" : "F");
    printf("%s: %s\n", ConvertString((unsigned char *)"换证次数", title), renew_count);
    printf("%s: %s\n", ConvertString((unsigned char *)"出生日期", title), birth_day);
    printf("%s: %s%s\n", ConvertString((unsigned char *)"英文姓名", title), english_name, english_name_ex);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件号码", title), id_number);
    printf("%s: %s\n", ConvertString((unsigned char *)"有效期起始日期", title), expire_start_day);
    printf("%s: %s\n", ConvertString((unsigned char *)"有效期截止日期", title), expire_end_day);
#endif
#ifdef WIN32
    dc_ParseOtherInfo(handle, 0x12, citizenship, info_buffer);
#else
    dc_ParseOtherInfo(handle, 0x22, citizenship, info_buffer);
#endif
#ifdef WIN32
    wprintf(L"%s: %s/%s\n", L"国籍", (wchar_t *)info_buffer, (wchar_t *)citizenship);
    wprintf(L"%s: %s\n", L"证件类型标识", (wchar_t *)type_sign);
    wprintf(L"%s: %s\n", L"既往版本外国人永久居留证件号码关联项", (wchar_t *)prev_related_info);
    if (wcslen((wchar_t *)prev_related_info) != 0) {
      wprintf(L"%s%s%s\n", L"持证人曾持有号码为", (wchar_t *)old_id_number, L"的外国人永久居留身份证");
    }
#else
    printf("%s: %s/%s\n", ConvertString((unsigned char *)"国籍", title), info_buffer, citizenship);
    printf("%s: %s\n", ConvertString((unsigned char *)"证件类型标识", title), type_sign);
    printf("%s: %s\n", ConvertString((unsigned char *)"既往版本外国人永久居留证件号码关联项", title), prev_related_info);
    if (strlen((char *)prev_related_info) != 0) {
      printf("%s%s%s\n", ConvertString((unsigned char *)"持证人曾持有号码为", title), old_id_number, ConvertString((unsigned char *)"的外国人永久居留身份证", title));
    }
#endif
  }

  printf("dc_ParsePhotoInfo ... ");
  result = dc_ParsePhotoInfo(handle, 0, photo_len, photo, 0, (unsigned char *)"me.bmp");
  if (result != 0) {
    if (result == -2) {
      printf("failed to call photo decoding library!\n");
    } else {
      printf("error!\n");
    }
    return -1;
  }
  printf("ok!\n");

  return 0;
}

static int SsCardTest(HANDLE handle) {
  char card_code[33];
  char card_type[2];
  char version[5];
  char init_org_number[25];
  char card_issue_date[9];
  char card_expire_day[9];
  char card_number[10];
  char social_security_number[19];
  char name[31];
  char name_ex[21];
  char sex[2];
  char nation[3];
  char birth_place[7];
  char birth_day[9];
  unsigned char title[128], message[512];

  printf("\n-------- SS Card Test --------\n");

  printf("dc_GetSocialSecurityCardBaseInfo ... ");
  if (dc_GetSocialSecurityCardBaseInfo(handle, 0, card_code, card_type, version, init_org_number, card_issue_date, card_expire_day, card_number, social_security_number, name, name_ex, sex, nation, birth_place, birth_day) != 0) {
    if (dc_GetSocialSecurityCardBaseInfo(handle, 1, card_code, card_type, version, init_org_number, card_issue_date, card_expire_day, card_number, social_security_number, name, name_ex, sex, nation, birth_place, birth_day) != 0) {
      printf("error!\n");
      return -1;
    }
  }
  printf("ok!\n");

  printf("%s: %s\n", ConvertString((unsigned char *)"卡的识别码", title), ConvertString((unsigned char *)card_code, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"卡的类别", title), ConvertString((unsigned char *)card_type, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"规范版本", title), ConvertString((unsigned char *)version, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"初始化机构编号", title), ConvertString((unsigned char *)init_org_number, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"发卡日期", title), ConvertString((unsigned char *)card_issue_date, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"卡有效期", title), ConvertString((unsigned char *)card_expire_day, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"卡号", title), ConvertString((unsigned char *)card_number, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"社会保障号码", title), ConvertString((unsigned char *)social_security_number, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"姓名", title), ConvertString((unsigned char *)name, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"姓名扩展", title), ConvertString((unsigned char *)name_ex, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"性别", title), ConvertString((unsigned char *)sex, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"民族", title), ConvertString((unsigned char *)nation, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"出生地", title), ConvertString((unsigned char *)birth_place, message));
  printf("%s: %s\n", ConvertString((unsigned char *)"出生日期", title), ConvertString((unsigned char *)birth_day, message));

  return 0;
}

static int SsCardPinTestStep1(HANDLE handle) {
  printf("\n-------- SS Card PIN Test (Step1) --------\n");

  printf("dc_ChangeSocialSecurityCardPassword ... ");
  if (dc_ChangeSocialSecurityCardPassword(handle, 1, "123456", "111111") != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  return 0;
}

static int SsCardPinTestStep2(HANDLE handle) {
  printf("\n-------- SS Card PIN Test (Step2) --------\n");

  printf("dc_ChangeSocialSecurityCardPassword ... ");
  if (dc_ChangeSocialSecurityCardPassword(handle, 1, "111111", "123456") != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  return 0;
}

static int BankCardTest(HANDLE handle, int type) {
  char number[128];

  printf("\n-------- Bank Card Test --------\n");

  printf("dc_GetBankAccountNumber ... ");
  if (dc_GetBankAccountNumber(handle, type, number) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("Account Number: %s\n", number);

  return 0;
}

static int M1CardTest(HANDLE handle) {
  unsigned char buffer[64], buffer2[64];
  unsigned int len;

  printf("\n-------- M1 Card Test --------\n");

  printf("dc_reset ... ");
  if (dc_reset(handle, 10) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_config_card ... ");
  if (dc_config_card(handle, 'A') != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_card_n_hex ... ");
  if (dc_card_n_hex(handle, 0x00, &len, buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("UID: %s\n", (char *)buffer);

  printf("dc_authentication_pass_hex (FFFFFFFFFFFF) ... ");
  if (dc_authentication_pass_hex(handle, 0x00, 0x00, (unsigned char *)"FFFFFFFFFFFF") != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_read_hex ... ");
  if (dc_read_hex(handle, 0x01, (char *)buffer2) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("Data: %s\n", (char *)buffer2);

  printf("dc_write_hex (55555555555555555555555555555555) ... ");
  if (dc_write_hex(handle, 0x01, (char *)"55555555555555555555555555555555") != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_read_hex ... ");
  if (dc_read_hex(handle, 0x01, (char *)buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("Data: %s\n", (char *)buffer);

  printf("dc_write_hex (11223344556677889900AABBCCDDEEFF) ... ");
  if (dc_write_hex(handle, 0x01, (char *)"11223344556677889900AABBCCDDEEFF") != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_read_hex ... ");
  if (dc_read_hex(handle, 0x01, (char *)buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("Data: %s\n", (char *)buffer);

  printf("dc_write_hex (%s) ... ", (char *)buffer2);
  if (dc_write_hex(handle, 0x01, (char *)buffer2) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_read_hex ... ");
  if (dc_read_hex(handle, 0x01, (char *)buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("Data: %s\n", (char *)buffer);

  return 0;
}

static int ContactlessCpuCardTest(HANDLE handle, unsigned char type) {
  unsigned char buffer[2048], buffer2[2048];
  unsigned char byte_len;
  unsigned int len, len2;

  printf("\n-------- Contactless CPU Card Test (Type %c) --------\n", type);

  printf("dc_reset ... ");
  if (dc_reset(handle, 10) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_config_card ... ");
  if (dc_config_card(handle, type) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  if (type == 'A') {
    printf("dc_card_n_hex ... ");
    if (dc_card_n_hex(handle, 0x00, &len, buffer) != 0) {
      printf("error!\n");
      return -1;
    }
    printf("ok!\n");

    printf("UID: %s\n", (char *)buffer);

    printf("dc_pro_resetInt_hex ... ");
    if (dc_pro_resetInt_hex(handle, &byte_len, (char *)buffer) != 0) {
      printf("error!\n");
      return -1;
    }
    printf("ok!\n");

    printf("ATS: %s\n", (char *)buffer);
  } else {
    printf("dc_card_b_hex ... ");
    if (dc_card_b_hex(handle, (char *)buffer) != 0) {
      printf("error!\n");
      return -1;
    }
    printf("ok!\n");

    printf("Info: %s\n", (char *)buffer);
  }

  printf("dc_pro_commandlinkInt_hex (APDU Request: 0084000008) ... ");
  strcpy((char *)buffer, "0084000008");
  len = 5;
  if (dc_pro_commandlinkInt_hex(handle, len, (char *)buffer, &len2, (char *)buffer2, 7) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("APDU Response: %s\n", (char *)buffer2);

  return 0;
}

static int ContactCpuCardTest(HANDLE handle, int number) {
  unsigned char buffer[2048], buffer2[2048];
  unsigned char byte_len;
  unsigned int len, len2;

  printf("\n-------- Contact CPU Card Test (Slot %d) --------\n", number);

  printf("dc_setcpu ... ");
  if (dc_setcpu(handle, 0x0C + number) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_cpureset_hex ... ");
  if (dc_cpureset_hex(handle, &byte_len, (char *)buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("ATR: %s\n", (char *)buffer);

  printf("dc_cpuapduInt_hex (APDU Request: 0084000008) ... ");
  strcpy((char *)buffer, "0084000008");
  len = 5;
  if (dc_cpuapduInt_hex(handle, len, (char *)buffer, &len2, (char *)buffer2) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("APDU Response: %s\n", (char *)buffer2);

  return 0;
}

static int Sle4442CardTest(HANDLE handle) {
  unsigned char buffer[2048];

  printf("\n-------- SLE4442 Card Test --------\n");

  printf("dc_Check_4442 ... ");
  if (dc_Check_4442(handle) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_read_4442_hex ... ");
  if (dc_read_4442_hex(handle, 0, 32, buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("Data: %s\n", (char *)buffer);

  return 0;
}

static int Sle4428CardTest(HANDLE handle) {
  unsigned char buffer[2048];

  printf("\n-------- SLE4428 Card Test --------\n");

  printf("dc_Check_4428 ... ");
  if (dc_Check_4428(handle) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_read_4428_hex ... ");
  if (dc_read_4428_hex(handle, 0, 32, buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("Data: %s\n", (char *)buffer);

  return 0;
}

static int EepromTest(HANDLE handle) {
  unsigned char buffer[2048];

  printf("\n-------- EEPROM Test --------\n");

  Random(64, buffer);

  printf("dc_swr_eeprom ... ");
  if (dc_swr_eeprom(handle, 0, 64, buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_srd_eeprom ... ");
  if (dc_srd_eeprom(handle, 0, 64, &buffer[64]) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  if (memcmp(buffer, &buffer[64], 64) != 0) {
    printf("check error!\n");
    return -1;
  }

  printf("check ok!\n");

  return 0;
}

static int EmidCardTest(HANDLE handle) {
  unsigned char buffer[2048];

  printf("\n-------- EMID Card Test --------\n");

  printf("dc_read_idcard_hex ... ");
  if (dc_read_idcard_hex(handle, 1, buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("Data: %s\n", (char *)buffer);

  return 0;
}

static int LcdDisplayTest(HANDLE handle) {
  unsigned char buffer[128];

  printf("\n-------- LCD Display Test --------\n");

  printf("dc_LcdDisplayText (First Line) ... ");
  if (dc_LcdDisplayText(handle, 0, 0, 0, 5, ConvertString(0, (unsigned char *)"  欢迎使用  ", buffer))) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_LcdDisplayText (Second Line Step1) ... ");
  if (dc_LcdDisplayText(handle, 1, 0, 0, 5, ConvertString(0, (unsigned char *)"编号：", buffer))) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_LcdDisplay (Second Line Step2) ... ");
  if (dc_LcdDisplay(handle, 1, 3, 0, "ABCxyz1234") != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_LcdDisplayText (Third Line Step1) ... ");
  if (dc_LcdDisplayText(handle, 2, 0, 0, 5, ConvertString(0, (unsigned char *)"姓名：", buffer))) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_LcdDisplay (Third Line Step2) ... ");
  if (dc_LcdDisplayText(handle, 2, 3, 0, 5, ConvertString(0, (unsigned char *)" 李小明 ", buffer))) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_LcdDisplayText (Fourth Line Step1) ... ");
  if (dc_LcdDisplayText(handle, 3, 0, 0, 5, ConvertString(0, (unsigned char *)"金额：", buffer))) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_LcdDisplay (Fourth Line Step2) ... ");
  if (dc_LcdDisplay(handle, 3, 3, 0, "88888.88") != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_LcdDisplayText (Fourth Line Step3) ... ");
  if (dc_LcdDisplayText(handle, 3, 7, 0, 5, ConvertString(0, (unsigned char *)"元", buffer))) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  return 0;
}

static int MultiInstanceTest(int port) {
  int result;
  HANDLE handle;
  unsigned char buffer[1024];

  printf("\n-------- Multi Instance Test --------\n");

  if (port == 100) {
    ++port;
  }

  printf("dc_init %d %d ... ", port, 115200);
  result = (int)dc_init(port, 115200);
  if (result < 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  handle = (HANDLE)result;

  printf("dc_getver ... ");
  result = dc_getver(handle, buffer);
  if (result != 0) {
    printf("error!\n");
    dc_exit(handle);
    return -1;
  }
  printf("ok!\n");

  printf("Firmware version: %s\n", (char *)buffer);

  dc_exit(handle);

  return 0;
}

static int DeviceSnTest(HANDLE handle) {
  unsigned char buffer[64];

  printf("\n-------- Device SN Test --------\n");

  printf("dc_readdevsnr ... ");
  if (dc_readdevsnr(handle, buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("Device SN: %s\n", (char *)buffer);

  return 0;
}

static int DeviceUidTest(HANDLE handle) {
  unsigned char buffer[64];

  printf("\n-------- Device UID Test --------\n");

  printf("dc_GetDeviceUid ... ");
  if (dc_GetDeviceUid(handle, (char *)buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("Device UID: %s\n", (char *)buffer);

  return 0;
}

static int SamaTest(HANDLE handle) {
  int result;
  unsigned char buffer[512];
  unsigned char t_data[32];
  unsigned char shortcode[16];
  unsigned char data_with_sign[2048];
  unsigned int data_with_sign_len;
  unsigned char photo[1024];
  unsigned int photo_len;
  unsigned char fingerprint[1024];
  unsigned int fingerprint_len;
  bool have_pf;
  unsigned char *activate_data;
  int activate_size;
  unsigned char state;
  unsigned int remain_number;
  unsigned char enable_time[14];
  unsigned char disable_time[14];
  unsigned char second_cert[1024];
  unsigned int second_cert_len;
  unsigned char encode_cert[1024];
  unsigned int encode_cert_len;
  unsigned char sign_cert[1024];
  unsigned int sign_cert_len;
#ifndef WIN32
  unsigned char buffer2[512];
#endif

  printf("\n-------- SAMA Test --------\n");

  printf("dc_SamAInit ... ");
  if (dc_SamAInit(handle) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_SamAReadSerialNumber ... ");
  if (dc_SamAReadSerialNumber(handle, (char *)buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("SAMA SN: %s\n", (char *)buffer);

  printf("dc_SamAStartFindIDCard ... ");
  if (dc_SamAStartFindIDCard(handle, buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("Management Number: %02X%02X%02X%02X\n", buffer[0], buffer[1], buffer[2], buffer[3]);

  printf("dc_SamASelectIDCard ... ");
  if (dc_SamASelectIDCard(handle, buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("Serial Number: %02X%02X%02X%02X%02X%02X%02X%02X\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7]);

  activate_size = ObtainFileData("activate.dat", &activate_data);
  if (activate_size < 0) {
    printf("No Activate Data!\n");
    return -1;
  }

  printf("dc_SamAGetEnaStat ... ");
  if (dc_SamAGetEnaStat(handle, &state, &remain_number, enable_time, disable_time, second_cert, &second_cert_len, encode_cert, &encode_cert_len, sign_cert, &sign_cert_len) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  if (state != 1) {
    printf("dc_SamAEnableSAM ... ");
    result = dc_SamAEnableSAM(handle, activate_data, activate_size);
    free(activate_data);
    if (result != 0) {
      printf("error!\n");
      return -1;
    }
    printf("ok!\n");
  }

  do {
    have_pf = true;
    printf("dc_SamAReadChkDataPF ... ");
    result = dc_SamAReadChkDataPF(handle, (unsigned char *)"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F", t_data, shortcode, data_with_sign, &data_with_sign_len, photo, &photo_len, fingerprint, &fingerprint_len);
    if (result != 0) {
      printf("error!\n");
      have_pf = false;
      printf("dc_SamAReadChkData ... ");
      result = dc_SamAReadChkData(handle, (unsigned char *)"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F", t_data, shortcode, data_with_sign, &data_with_sign_len);
      if (result != 0) {
        printf("error!\n");
        break;
      }
    }
    printf("ok!\n");
  } while (false);

  if (result != 0) {
    return -1;
  }

  memset(buffer, 0, sizeof(buffer));
  memcpy(buffer, t_data, 32);
#ifdef WIN32
    wprintf(L"%s: %s\n", L"身份证有效期", (wchar_t *)buffer);
#else
    dc_string_converter(4, buffer, buffer2);
    printf("%s: %s\n", ConvertString((unsigned char *)"身份证有效期", buffer), buffer2);
#endif

  if (have_pf) {
    printf("dc_ParsePhotoInfo ... ");
    result = dc_ParsePhotoInfo(handle, 0, photo_len, photo, 0, (unsigned char *)"me.bmp");
    if (result != 0) {
      if (result == -2) {
        printf("failed to call photo decoding library!\n");
      } else {
        printf("error!\n");
      }
      return -1;
    }
    printf("ok!\n");
  }

  return 0;
}

static int MagneticCardTest(HANDLE handle) {
  unsigned char t1[512], t2[512], t3[512];
  unsigned int l1, l2, l3;

  printf("\n-------- Magnetic Card Test --------\n");

  printf("dc_readmagcardallA ... ");
  if (dc_readmagcardallA(handle, 20, t1, &l1, t2, &l2, t3, &l3) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("Track1 Data: %s\n", (char *)t1);
  printf("Track2 Data: %s\n", (char *)t2);
  printf("Track3 Data: %s\n", (char *)t3);

  return 0;
}

static int BarcodeTest(HANDLE handle) {
  unsigned char buffer[4096], buffer2[4096];
  int len;
  int time_ms;
  BigInt ms;

  printf("\n-------- Barcode Test --------\n");

  time_ms = 20000;

  printf("dc_Scan2DBarcodeStart ... ");
  if (dc_Scan2DBarcodeStart(handle, 0) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_Scan2DBarcodeGetData ... ");
  do {
    ms = GetCurrentTimeTick();
    if (dc_Scan2DBarcodeGetData(handle, &len, buffer) == 0) {
      printf("ok!\n");
      memset(buffer2, 0, sizeof(buffer2));
      memcpy(buffer2, buffer, len);
#ifdef WIN32
      dc_string_converter(5, buffer2, buffer);
      wprintf(L"Info: %s\n", (wchar_t *)buffer);
#else
      printf("Info: %s\n", buffer2);
#endif
      break;
    }
  } while ((time_ms -= static_cast<int>(GetCurrentTimeTick() - ms)) > 0);

  dc_Scan2DBarcodeExit(handle);

  if (time_ms <= 0) {
    printf("error!\n");
    return -1;
  }

  return 0;
}

static int Iso15693CardTest(HANDLE handle) {
  unsigned char len;
  unsigned char buffer[1024];
  unsigned char uid[8];
  unsigned char i, j;

  printf("\n-------- ISO15693 Card Test --------\n");

  printf("dc_reset ... ");
  if (dc_reset(handle, 10) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_config_card ... ");
  if (dc_config_card(handle, '1') != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_inventory ... ");
  if (dc_inventory(handle, 0x36, 0x00, 0x00, &len, buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  if (len < 9) {
    printf("data format error!\n");
    return -1;
  }

  printf("DSFID: %02X\n", buffer[0]);
  printf("UID: %02X %02X %02X %02X %02X %02X %02X %02X\n", buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8]);

  memcpy(uid, &buffer[1], 8);

  printf("******** Start Read Block Data ********\n");
  for (i = 0; i < 28; ++i) {
    if (dc_readblock(handle, 0x22, i, 1, uid, &len, buffer) == 0) {
      printf("Block Number %02d:", i);
      for (j = 0; j < len; ++j) {
        printf(" %02X", buffer[j]);
      }
      printf("\n");
    } else {
      printf("Block Number %02d: error.\n", i);
    }
  }
  printf("******** End Read Block Data ********\n");

  return 0;
}

static int ExternalKeypadTest(HANDLE handle) {
  int result;
  unsigned char buffer[1024];
  unsigned char state;
  int time_ms;
  BigInt ms;

  printf("\n-------- External Keypad Test --------\n");

  SD_IFD_ExitInput(handle);

  printf("SD_IFD_GetVersion ... ");
  if (SD_IFD_GetVersion(handle, (char *)buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("Keypad version: %s\n", (char *)buffer);

  printf("SD_IFD_GetPINPro ... ");
  if (SD_IFD_GetPINPro(handle, (char *)buffer, 0x82, 20) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("Plaintext PIN: %s\n", (char *)buffer);

  printf("SD_IFD_GetEnPINPro ... ");
  if (SD_IFD_GetEnPINPro(handle, (char *)buffer, 0x30, 0x31, 20) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("Ciphertext PIN: %s\n", (char *)buffer);

  printf("SD_IFD_GetAmount ... ");
  if (SD_IFD_GetAmount(handle, (char *)buffer, 20) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("Amount String: %s\n", (char *)buffer);

  time_ms = 60000;

  printf("SD_IFD_StartInput ... ");
  if (SD_IFD_StartInput(handle, 0x30) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("SD_IFD_GetInputValue ... ");
  do {
    ms = GetCurrentTimeTick();
    result = SD_IFD_GetInputValue(handle, &state, (char *)buffer);
    if (result < 0) {
      printf("error!\n");
      SD_IFD_ExitInput(handle);
      return -1;
    }
    if (result == 0xA1) {
      printf("cancelled!\n");
      SD_IFD_ExitInput(handle);
      return -1;
    }
    if ((result == 0x00) && (state == 0x00)) {
      printf("ok!\n");
      break;
    }
  } while ((time_ms -= static_cast<int>(GetCurrentTimeTick() - ms)) > 0);

  SD_IFD_ExitInput(handle);

  if (time_ms <= 0) {
    printf("timeout!\n");
    return -1;
  }

  printf("Amount String: %s\n", (char *)buffer);

  time_ms = 60000;

  printf("SD_IFD_StartInput ... ");
  if (SD_IFD_StartInput(handle, 0x31) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("SD_IFD_GetInputValue ... ");
  do {
    ms = GetCurrentTimeTick();
    result = SD_IFD_GetInputValue(handle, &state, (char *)buffer);
    if (result < 0) {
      printf("error!\n");
      SD_IFD_ExitInput(handle);
      return -1;
    }
    if (result == 0xA1) {
      printf("cancelled!\n");
      SD_IFD_ExitInput(handle);
      return -1;
    }
    if ((result == 0x00) && (memcmp(buffer, "0000000", 7) != 0)) {
      printf("ok!\n");
      break;
    }
  } while ((time_ms -= static_cast<int>(GetCurrentTimeTick() - ms)) > 0);

  SD_IFD_ExitInput(handle);

  if (time_ms <= 0) {
    printf("timeout!\n");
    return -1;
  }

  if (memcmp(&buffer[5], "70", 2) == 0) {
    printf("Control Key: %s\n", "[F1]");
  } else if (memcmp(&buffer[5], "71", 2) == 0) {
    printf("Control Key: %s\n", "[F2]");
  } else if (memcmp(&buffer[5], "72", 2) == 0) {
    printf("Control Key: %s\n", "[F3]");
  } else if (memcmp(&buffer[5], "1B", 2) == 0) {
    printf("Control Key: %s\n", "[CANCEL]");
  } else if (memcmp(&buffer[5], "0D", 2) == 0) {
    printf("Control Key: %s\n", "[ENTER]");
  } else {
    printf("error!\n");
    return -1;
  }

  printf("SD_IFD_CreateMacPro ... ");
  if (SD_IFD_CreateMacPro(handle, 0x01, 34, (unsigned char *)"\xAB\xCD\xEF\x12\x34\x56\x78\x90\xAA\xBB\xCC\xDD\xEE\xFF\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xAB\xCD\xEF\x12\x34\x56\x78\x90\xAA\xFF", (char *)buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("MAC: %s\n", (char *)buffer);

  return 0;
}

static int NtagCardTest(HANDLE handle) {
  unsigned char buffer[64], buffer2[64];
  unsigned int len;

  printf("\n-------- Ntag Card Test --------\n");

  printf("dc_reset ... ");
  if (dc_reset(handle, 10) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_config_card ... ");
  if (dc_config_card(handle, 'A') != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_card_n_hex ... ");
  if (dc_card_n_hex(handle, 0x00, &len, buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("UID: %s\n", (char *)buffer);

  printf("dc_RfCrcModeConfig ... ");
  if (dc_RfCrcModeConfig(handle, 1, 0) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_read2_hex ... ");
  if (dc_read2_hex(handle, 0xFF, 4, 4, (char *)buffer2) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  buffer2[8] = 0x00;
  printf("Data: %s\n", (char *)buffer2);

  printf("dc_write2_hex (55555555) ... ");
  if (dc_write2_hex(handle, 0xFF, 4, 4, "55555555") != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_read2_hex ... ");
  if (dc_read2_hex(handle, 0xFF, 4, 4, (char *)buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  buffer[8] = 0x00;
  printf("Data: %s\n", (char *)buffer);

  printf("dc_write2_hex (11223344) ... ");
  if (dc_write2_hex(handle, 0xFF, 4, 4, "11223344") != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_read2_hex ... ");
  if (dc_read2_hex(handle, 0xFF, 4, 4, (char *)buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  buffer[8] = 0x00;
  printf("Data: %s\n", (char *)buffer);

  printf("dc_write2_hex (%s) ... ", (char *)buffer2);
  if (dc_write2_hex(handle, 0xFF, 4, 4, (char *)buffer2) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  printf("dc_read2_hex ... ");
  if (dc_read2_hex(handle, 0xFF, 4, 4, (char *)buffer) != 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  buffer[8] = 0x00;
  printf("Data: %s\n", (char *)buffer);

  return 0;
}

class AutoPause {
 public:
  AutoPause() {}
  ~AutoPause() {
    printf("\nPress ENTER key to exit ...\n");
    getchar();
  }
};

static int Options(int argc, char *argv[], char *device_name, int *port, char *port_name, int *baud, int *type) {
  int result = 0;
  int i;

  for (i = 1; i < argc; ++i) {
    if ((strcmp(argv[i], "-?") == 0) || (strcmp(argv[i], "--help") == 0)) {
      return -1;
    }
    if ((strcmp(argv[i], "-n") == 0) && (i + 1 < argc)) {
      strcpy(device_name, argv[++i]);
      ++result;
    } else if ((strcmp(argv[i], "-p") == 0) && (i + 1 < argc)) {
      *port = atoi(argv[++i]);
      ++result;
    } else if ((strcmp(argv[i], "-c") == 0) && (i + 1 < argc)) {
      strcpy(port_name, argv[++i]);
      ++result;
    } else if ((strcmp(argv[i], "-b") == 0) && (i + 1 < argc)) {
      *baud = atoi(argv[++i]);
      ++result;
    } else if ((strcmp(argv[i], "-t") == 0) && (i + 1 < argc)) {
      *type = atoi(argv[++i]);
      ++result;
    } else {
      return -1;
    }
  }

  return result;
}

int main(int argc, char *argv[]) {
#ifdef WIN32
  AutoPause auto_pause;
#endif
  char device_name[64] = {0};
  int port = 100;
  char port_name[256] = {0};
  int baud = 115200;
  int type = 0;
  int result;
  HANDLE handle;
  unsigned char buffer[1024];
  char *p;

  setlocale(LC_ALL, "");

  printf("Program path: %s\n", argv[0]);

  LibMain(0, buffer);
  printf("Library version: %s\n", (char *)buffer);

  strcpy((char *)buffer, argv[0]);
#ifdef WIN32
  p = strrchr((char *)buffer, '\\');
#else
  p = strrchr((char *)buffer, '/');
#endif
  if (p) {
    p[1] = '\0';
    LibMain(1, buffer);
    LibMain(2, buffer);
  }

  if (Options(argc, argv, device_name, &port, port_name, &baud, &type) < 0) {
    printf("NAME\n\tdcrf32test - test dcrf32 library\n\n");
    printf("SYNOPSIS\n\tdcrf32test [option]...\n\n");
    printf("DESCRIPTION\n\t-?, --help\n\t\tPrint a usage summary and exit.\n");
    printf("\t-n <devicename>\n\t\tDevice name.\n");
    printf("\t-p <port>\n\t\tCommunication port, default value is 100.\n");
    printf("\t\t0~99        - COM Port\n");
    printf("\t\t100~199     - USB Port\n");
    printf("\t\t200~299     - PCSC Port\n");
    printf("\t\t300~399     - Bluetooth Port\n");
    printf("\t\t400~499     - VNet Port\n");
    printf("\t\t500~599     - SPI Port\n");
    printf("\t\t600~699     - TCP Port\n");
    printf("\t\t700~799     - FTDI Port\n");
    printf("\t-c <portname>\n\t\tPhysical name corresponding to communication port.\n");
    printf("\t-b <baud>\n\t\tBaud rate, default value is 115200.\n");
    printf("\t-t <type>\n\t\tTest category, default value is 0.\n");
    printf("\t\t0           - Device Connectivity Test\n");
    printf("\t\t1           - ID Card Test\n");
    printf("\t\t2           - ID Card Test (Hardware Acceleration)\n");
    printf("\t\t3           - SS Card Test\n");
    printf("\t\t4           - SS Card PIN Test (Step1)\n");
    printf("\t\t5           - SS Card PIN Test (Step2)\n");
    printf("\t\t6           - Bank Card Test (Contactless)\n");
    printf("\t\t7           - Bank Card Test (Contact)\n");
    printf("\t\t8           - M1 Card Test\n");
    printf("\t\t9           - Contactless CPU Card Test (Type A)\n");
    printf("\t\t10          - Contactless CPU Card Test (Type B)\n");
    printf("\t\t11          - Contact CPU Card Test (Master Slot)\n");
    printf("\t\t12          - Contact CPU Card Test (Sam1 Slot)\n");
    printf("\t\t13          - Contact CPU Card Test (Sam2 Slot)\n");
    printf("\t\t14          - Contact CPU Card Test (Sam3 Slot)\n");
    printf("\t\t15          - SLE4442 Card Test\n");
    printf("\t\t16          - SLE4428 Card Test\n");
    printf("\t\t17          - EEPROM Test\n");
    printf("\t\t18          - EMID Card Test\n");
    printf("\t\t19          - LCD Display Test\n");
    printf("\t\t20          - Multi Instance Test\n");
    printf("\t\t21          - Device SN Test\n");
    printf("\t\t22          - Device UID Test\n");
    printf("\t\t23          - SAMA Test\n");
    printf("\t\t24          - Magnetic Card Test\n");
    printf("\t\t25          - Barcode Test\n");
    printf("\t\t26          - ISO15693 Card Test\n");
    printf("\t\t27          - External Keypad Test\n");
    printf("\t\t28          - Ntag Card Test\n");
    return 0;
  }

  printf("devicename: %s\n", device_name);
  printf("port: %d\n", port);
  printf("portname: %s\n", port_name);
  printf("baud: %d\n", baud);
  printf("type: %d\n", type);

  if (strcmp(port_name, "") != 0) {
    printf("dc_config_port_name %d %s ... ", port, port_name);
    dc_config_port_name(port, port_name);
    printf("ok!\n");
  }

  if (strcmp(device_name, "") == 0) {
    printf("dc_init %d %d ... ", port, baud);
    result = (int)dc_init(port, baud);
  } else {
    printf("dc_init_name %d %d %s ... ", port, baud, device_name);
    result = (int)dc_init_name(port, baud, device_name);
  }
  if (result < 0) {
    printf("error!\n");
    return -1;
  }
  printf("ok!\n");

  handle = (HANDLE)result;

  printf("dc_getver ... ");
  result = dc_getver(handle, buffer);
  if (result != 0) {
    printf("error!\n");
    dc_exit(handle);
    return -1;
  }
  printf("ok!\n");

  printf("Firmware version: %s\n", (char *)buffer);

  switch (type) {
    case 0:
      break;
    case 1:
      result = IdCardTest(handle);
      break;
    case 2:
      result = IdCardTest2(handle);
      break;
    case 3:
      result = SsCardTest(handle);
      break;
    case 4:
      result = SsCardPinTestStep1(handle);
      break;
    case 5:
      result = SsCardPinTestStep2(handle);
      break;
    case 6:
      result = BankCardTest(handle, 0);
      break;
    case 7:
      result = BankCardTest(handle, 1);
      break;
    case 8:
      result = M1CardTest(handle);
      break;
    case 9:
      result = ContactlessCpuCardTest(handle, 'A');
      break;
    case 10:
      result = ContactlessCpuCardTest(handle, 'B');
      break;
    case 11:
      result = ContactCpuCardTest(handle, 0);
      break;
    case 12:
      result = ContactCpuCardTest(handle, 1);
      break;
    case 13:
      result = ContactCpuCardTest(handle, 2);
      break;
    case 14:
      result = ContactCpuCardTest(handle, 3);
      break;
    case 15:
      result = Sle4442CardTest(handle);
      break;
    case 16:
      result = Sle4428CardTest(handle);
      break;
    case 17:
      result = EepromTest(handle);
      break;
    case 18:
      result = EmidCardTest(handle);
      break;
    case 19:
      result = LcdDisplayTest(handle);
      break;
    case 20:
      result = MultiInstanceTest(port);
      break;
    case 21:
      result = DeviceSnTest(handle);
      break;
    case 22:
      result = DeviceUidTest(handle);
      break;
    case 23:
      result = SamaTest(handle);
      break;
    case 24:
      result = MagneticCardTest(handle);
      break;
    case 25:
      result = BarcodeTest(handle);
      break;
    case 26:
      result = Iso15693CardTest(handle);
      break;
    case 27:
      result = ExternalKeypadTest(handle);
      break;
    case 28:
      result = NtagCardTest(handle);
      break;
    default:
      printf("Unsupported test category.\n");
      result = -1;
      break;
  }

  if (result != 0) {
    dc_exit(handle);
    return -1;
  }

  dc_beep(handle, 20);

  dc_exit(handle);

  return 0;
}
