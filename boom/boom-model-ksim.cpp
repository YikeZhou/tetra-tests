#include "boom-model.h"
#include "boom-ksim.h"
#include <iostream>

namespace {
class KsimBoomModel : public BoomModel {
  bool clock;

public:
  KsimBoomModel() {
    name = "ksim";
    clock = false;
  }
  ~KsimBoomModel() {}

  void vcd_start(const char *outputFile) override {} // TODO
  void vcd_dump(size_t t) override {} // TODO

  void eval() override {
    if (clock) {
      BoomSystem();
    } else {
      BoomSystem__comb();
    }
  }

  Ports get_ports() override {
    return {
#define PORT(name) name,
#include "ports.def"
    };
  }

  void set_reset(bool _reset) override { reset = _reset; }
  void set_clock(bool _clock) override { clock = _clock; }

  void set_mem(AxiInputs &in) override {
    mem_axi4_0_aw_ready = in.aw_ready;
    mem_axi4_0_w_ready = in.w_ready;
    mem_axi4_0_b_valid = in.b_valid;
    mem_axi4_0_b_bits_id = in.b_id;
    mem_axi4_0_b_bits_resp = in.b_resp;
    mem_axi4_0_ar_ready = in.ar_ready;
    mem_axi4_0_r_valid = in.r_valid;
    mem_axi4_0_r_bits_id = in.r_id;
    mem_axi4_0_r_bits_data = in.r_data;
    mem_axi4_0_r_bits_resp = in.r_resp;
    mem_axi4_0_r_bits_last = in.r_last;
  }

  AxiOutputs get_mem() override {
    AxiOutputs out;
    out.aw_valid = mem_axi4_0_aw_valid;
    out.aw_id = mem_axi4_0_aw_bits_id;
    out.aw_addr = mem_axi4_0_aw_bits_addr;
    out.aw_len = mem_axi4_0_aw_bits_len;
    out.aw_size = mem_axi4_0_aw_bits_size;
    out.w_valid = mem_axi4_0_w_valid;
    out.w_data = mem_axi4_0_w_bits_data;
    out.w_strb = mem_axi4_0_w_bits_strb;
    out.w_last = mem_axi4_0_w_bits_last;
    out.b_ready = mem_axi4_0_b_ready;
    out.ar_valid = mem_axi4_0_ar_valid;
    out.ar_id = mem_axi4_0_ar_bits_id;
    out.ar_addr = mem_axi4_0_ar_bits_addr;
    out.ar_len = mem_axi4_0_ar_bits_len;
    out.ar_size = mem_axi4_0_ar_bits_size;
    out.r_ready = mem_axi4_0_r_ready;
    return out;
  }

  void set_mmio(AxiInputs &in) override {
    mmio_axi4_0_aw_ready = in.aw_ready;
    mmio_axi4_0_w_ready = in.w_ready;
    mmio_axi4_0_b_valid = in.b_valid;
    mmio_axi4_0_b_bits_id = in.b_id;
    mmio_axi4_0_b_bits_resp = in.b_resp;
    mmio_axi4_0_ar_ready = in.ar_ready;
    mmio_axi4_0_r_valid = in.r_valid;
    mmio_axi4_0_r_bits_id = in.r_id;
    mmio_axi4_0_r_bits_data = in.r_data;
    mmio_axi4_0_r_bits_resp = in.r_resp;
    mmio_axi4_0_r_bits_last = in.r_last;
  }

  AxiOutputs get_mmio() override {
    AxiOutputs out;
    out.aw_valid = mmio_axi4_0_aw_valid;
    out.aw_id = mmio_axi4_0_aw_bits_id;
    out.aw_addr = mmio_axi4_0_aw_bits_addr;
    out.aw_len = mmio_axi4_0_aw_bits_len;
    out.aw_size = mmio_axi4_0_aw_bits_size;
    out.w_valid = mmio_axi4_0_w_valid;
    out.w_data = mmio_axi4_0_w_bits_data;
    out.w_strb = mmio_axi4_0_w_bits_strb;
    out.w_last = mmio_axi4_0_w_bits_last;
    out.b_ready = mmio_axi4_0_b_ready;
    out.ar_valid = mmio_axi4_0_ar_valid;
    out.ar_id = mmio_axi4_0_ar_bits_id;
    out.ar_addr = mmio_axi4_0_ar_bits_addr;
    out.ar_len = mmio_axi4_0_ar_bits_len;
    out.ar_size = mmio_axi4_0_ar_bits_size;
    out.r_ready = mmio_axi4_0_r_ready;
    return out;
  }
};
} // namespace

std::unique_ptr<BoomModel> makeKsimModel() {
  return std::make_unique<KsimBoomModel>();
}