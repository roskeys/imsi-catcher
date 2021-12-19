/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2021 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#include "srsue/hdr/phy/nr/cell_search.h"
#include "srsran/common/band_helper.h"
#include "srsran/common/buffer_pool.h"
#include "srsran/radio/rf_buffer.h"
#include "srsran/radio/rf_timestamp.h"

namespace srsue {
namespace nr {
cell_search::cell_search(srslog::basic_logger& logger_) : logger(logger_) {}

cell_search::~cell_search()
{
  srsran_ssb_free(&ssb);
}

bool cell_search::init(const args_t& args)
{
  // Prepare SSB initialization arguments
  srsran_ssb_args_t ssb_args = {};
  ssb_args.max_srate_hz      = args.max_srate_hz;
  ssb_args.min_scs           = args.ssb_min_scs;
  ssb_args.enable_search     = true;
  ssb_args.enable_decode     = true;

  // Initialise SSB
  if (srsran_ssb_init(&ssb, &ssb_args) < SRSRAN_SUCCESS) {
    logger.error("Cell search: Error initiating SSB");
    return false;
  }

  return true;
}

bool cell_search::start(const cfg_t& cfg)
{
  // Prepare SSB configuration
  srsran_ssb_cfg_t ssb_cfg = {};
  ssb_cfg.srate_hz         = cfg.srate_hz;
  ssb_cfg.center_freq_hz   = cfg.center_freq_hz;
  ssb_cfg.ssb_freq_hz      = cfg.ssb_freq_hz;
  ssb_cfg.scs              = cfg.ssb_scs;
  ssb_cfg.pattern          = cfg.ssb_pattern;
  ssb_cfg.duplex_mode      = cfg.duplex_mode;

  // Configure SSB
  if (srsran_ssb_set_cfg(&ssb, &ssb_cfg) < SRSRAN_SUCCESS) {
    logger.error("Cell search: Error setting SSB configuration");
    return false;
  }
  return true;
}

cell_search::ret_t cell_search::run_slot(const cf_t* buffer, uint32_t slot_sz)
{
  cell_search::ret_t ret = {};

  // Search for SSB
  if (srsran_ssb_search(&ssb, buffer, slot_sz + ssb.ssb_sz, &ret.ssb_res) < SRSRAN_SUCCESS) {
    logger.error("Error occurred searching SSB");
    ret.result = ret_t::ERROR;
  } else if (ret.ssb_res.measurements.snr_dB >= -10.0f and ret.ssb_res.pbch_msg.crc) {
    // Consider the SSB is found and decoded if the PBCH CRC matched
    ret.result = ret_t::CELL_FOUND;
  } else {
    ret.result = ret_t::CELL_NOT_FOUND;
  }
  return ret;
}

} // namespace nr
} // namespace srsue