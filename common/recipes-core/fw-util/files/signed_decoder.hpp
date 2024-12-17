#pragma once
#include "fw-util.h"

#define MD5_SIZE          (16)
#define PROJECT_CODE_SIZE (16)
#define FW_VERSION_SIZE   (13)
#define ERROR_PROOF       (3)
#define SIGN_INFO_SIZE    (MD5_SIZE+PROJECT_CODE_SIZE+FW_VERSION_SIZE+ERROR_PROOF+MD5_SIZE)

enum FORMAT_ERR {
  SUCCESS = 0,
  INVALID_SIGNATURE,
  INVALID_FILE,
  DECODE_FAIL
};

enum MD5_ERR {
  MD5_SUCCESS = 0,
  SIZE_ERROR,
  FILE_ERROR,
  MD5_INIT_ERROR,
  MD5_UPDATE_ERROR,
  MD5_FINAL_ERROR,
  MD5_CHECKSUM_ERROR,
};

enum INFO_ERR {
  EP_SUCCESS = 0,
  PROJECT_NAME_NOT_MATCH,
  BOARD_NOT_MATCH,
  STAGE_NOT_MATCH,
  COMPONENT_NOT_MATCH,
  SOURCE_NOT_MATCH,
};

const uint8_t COMPONENT_VERIFY_SKIPPED = 0xFF;

/*
 * Signed Image would be => Image + Sign_info
 *
 * Sign_info :
 *   MD5 - 1      (16 bytes)
 *   Project Name (16 bytes)
 *   Firmware Ver (13 bytes)
 *   Error Proof  ( 3 bytes)
 *   MD5 - 2      (16 bytes)
 *
 * MD5 - 1 calculate from Image
 * MD5 - 2 calculate from Sign_info (without MD5 - 2)
 * Error Proof:
 *   Board id     [ 4: 0]
 *   Stage id     [ 7: 5]
 *   Component id [15: 8]
 *   Vendor id    [23:16] (Source id)
 */

struct signed_header_t {
  std::string project_name{};
  uint8_t board_id{};
  uint8_t stage_id{};
  uint8_t component_id{};
  uint8_t vendor_id{};

  signed_header_t(){}
  signed_header_t(const std::string& pn, uint8_t bid, uint8_t sid, uint8_t cid, uint8_t vid):
            project_name(pn), board_id(bid), stage_id(sid), component_id(cid), vendor_id(vid){}
  signed_header_t(const signed_header_t& info, uint8_t component_id):
            project_name(info.project_name), board_id(info.board_id), stage_id(info.stage_id),
            component_id(component_id), vendor_id(info.vendor_id){}
  signed_header_t(const signed_header_t& info, uint8_t component_id, uint8_t vendor_id):
            project_name(info.project_name), board_id(info.board_id), stage_id(info.stage_id),
            component_id(component_id), vendor_id(vendor_id){}
};

// NOTE: pldm_image_signed_info_map is used to convert vendor-defined descriptor
// strings in the PLDM FW image to numbers.
using str_uint8_t_map_t = std::unordered_map<std::string, uint8_t>;
struct pldm_image_signed_info_map {
  str_uint8_t_map_t board_id;
  str_uint8_t_map_t stage;
  str_uint8_t_map_t component_id;
  str_uint8_t_map_t vendor_id;
};

class InfoChecker {
  protected:
    signed_header_t comp_info{};
    static int check_md5(const std::string& image_path, long offset, long size, uint8_t* data);
    int check_header_info(const signed_header_t&);

  public:
    explicit InfoChecker(const signed_header_t& info):comp_info(info){}
};

struct FW_IMG_INFO {
  uint8_t MD5_1[MD5_SIZE];
  uint8_t Project_Code[PROJECT_CODE_SIZE];
  uint8_t FW_Version[FW_VERSION_SIZE];
  uint8_t Error_Proof[ERROR_PROOF];
  uint8_t MD5_2[MD5_SIZE];
};

class SignComponent : public InfoChecker
{
  private:
    size_t image_size = 0;
    uint8_t md5[MD5_SIZE] = {0};
    std::string temp_image_path{};
    static void trim_function(std::string& str);
  protected:
    static signed_header_t get_info(FW_IMG_INFO&);
    int is_image_signed(const std::string& image_path, bool force); // check signed info and store.
    int get_image(std::string& /*image*/, bool /*force*/); // would change original file path to temp file path.
    int delete_image();

  public:
    SignComponent(const signed_header_t& info, const std::string &fru) :
                InfoChecker(info), temp_image_path("/tmp/" + fru + "_signed_update.bin") {}
    virtual ~SignComponent() = default;
    virtual int component_update(const std::string& /*image*/, bool /*force*/) {return FW_STATUS_NOT_SUPPORTED;};
    int signed_image_update(std::string /*image*/, bool /*force*/);
};
