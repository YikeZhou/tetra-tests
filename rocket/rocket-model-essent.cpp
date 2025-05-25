#include "rocket-model.h"
#include "rocket-essent.h"
#include <iostream>
#include "uint.h"

namespace {
class EssentRocketModel : public RocketModel {
  RocketSystem model;
  bool clock;

public:
  EssentRocketModel() {
    name = "essent";
    clock = false;
  }
  ~EssentRocketModel() {}

  void vcd_start(const char *outputFile) override {} // TODO
  void vcd_dump(size_t t) override {} // TODO

  void eval() override {
    bool update_registers = clock, verbose = false, done_reset = model.reset.as_single_word() == 0;
    // NOTE: using done_reset will cause internal assertion failure in essent
    model.eval(update_registers, verbose, false);
  }

  Ports get_ports() override {
    return {
#define PORT(name) model.name.as_single_word(),
#include "ports.def"
    };
  }

  void set_reset(bool reset) override { model.reset = UInt<1>(reset); }
  void set_clock(bool _clock) override { clock = _clock; }

  void set_mem(AxiInputs &in) override {
    model.mem_axi4_0_aw_ready = UInt<1>(in.aw_ready);
    model.mem_axi4_0_w_ready = UInt<1>(in.w_ready);
    model.mem_axi4_0_b_valid = UInt<1>(in.b_valid);
    model.mem_axi4_0_b_bits_id = UInt<4>(in.b_id);
    model.mem_axi4_0_b_bits_resp = UInt<2>(in.b_resp);
    model.mem_axi4_0_ar_ready = UInt<1>(in.ar_ready);
    model.mem_axi4_0_r_valid = UInt<1>(in.r_valid);
    model.mem_axi4_0_r_bits_id = UInt<4>(in.r_id);
    model.mem_axi4_0_r_bits_data = UInt<64>(in.r_data);
    model.mem_axi4_0_r_bits_resp = UInt<2>(in.r_resp);
    model.mem_axi4_0_r_bits_last = UInt<1>(in.r_last);
  }

  AxiOutputs get_mem() override {
    AxiOutputs out;
    out.aw_valid = model.mem_axi4_0_aw_valid.as_single_word();
    out.aw_id = model.mem_axi4_0_aw_bits_id.as_single_word();
    out.aw_addr = model.mem_axi4_0_aw_bits_addr.as_single_word();
    out.aw_len = model.mem_axi4_0_aw_bits_len.as_single_word();
    out.aw_size = model.mem_axi4_0_aw_bits_size.as_single_word();
    out.w_valid = model.mem_axi4_0_w_valid.as_single_word();
    out.w_data = model.mem_axi4_0_w_bits_data.as_single_word();
    out.w_strb = model.mem_axi4_0_w_bits_strb.as_single_word();
    out.w_last = model.mem_axi4_0_w_bits_last.as_single_word();
    out.b_ready = model.mem_axi4_0_b_ready.as_single_word();
    out.ar_valid = model.mem_axi4_0_ar_valid.as_single_word();
    out.ar_id = model.mem_axi4_0_ar_bits_id.as_single_word();
    out.ar_addr = model.mem_axi4_0_ar_bits_addr.as_single_word();
    out.ar_len = model.mem_axi4_0_ar_bits_len.as_single_word();
    out.ar_size = model.mem_axi4_0_ar_bits_size.as_single_word();
    out.r_ready = model.mem_axi4_0_r_ready.as_single_word();
    return out;
  }

  void set_mmio(AxiInputs &in) override {
    model.mmio_axi4_0_aw_ready = UInt<1>(in.aw_ready);
    model.mmio_axi4_0_w_ready = UInt<1>(in.w_ready);
    model.mmio_axi4_0_b_valid = UInt<1>(in.b_valid);
    model.mmio_axi4_0_b_bits_id = UInt<4>(in.b_id);
    model.mmio_axi4_0_b_bits_resp = UInt<2>(in.b_resp);
    model.mmio_axi4_0_ar_ready = UInt<1>(in.ar_ready);
    model.mmio_axi4_0_r_valid = UInt<1>(in.r_valid);
    model.mmio_axi4_0_r_bits_id = UInt<4>(in.r_id);
    model.mmio_axi4_0_r_bits_data = UInt<64>(in.r_data);
    model.mmio_axi4_0_r_bits_resp = UInt<2>(in.r_resp);
    model.mmio_axi4_0_r_bits_last = UInt<1>(in.r_last);
  }

  AxiOutputs get_mmio() override {
    AxiOutputs out;
    out.aw_valid = model.mmio_axi4_0_aw_valid.as_single_word();
    out.aw_id = model.mmio_axi4_0_aw_bits_id.as_single_word();
    out.aw_addr = model.mmio_axi4_0_aw_bits_addr.as_single_word();
    out.aw_len = model.mmio_axi4_0_aw_bits_len.as_single_word();
    out.aw_size = model.mmio_axi4_0_aw_bits_size.as_single_word();
    out.w_valid = model.mmio_axi4_0_w_valid.as_single_word();
    out.w_data = model.mmio_axi4_0_w_bits_data.as_single_word();
    out.w_strb = model.mmio_axi4_0_w_bits_strb.as_single_word();
    out.w_last = model.mmio_axi4_0_w_bits_last.as_single_word();
    out.b_ready = model.mmio_axi4_0_b_ready.as_single_word();
    out.ar_valid = model.mmio_axi4_0_ar_valid.as_single_word();
    out.ar_id = model.mmio_axi4_0_ar_bits_id.as_single_word();
    out.ar_addr = model.mmio_axi4_0_ar_bits_addr.as_single_word();
    out.ar_len = model.mmio_axi4_0_ar_bits_len.as_single_word();
    out.ar_size = model.mmio_axi4_0_ar_bits_size.as_single_word();
    out.r_ready = model.mmio_axi4_0_r_ready.as_single_word();
    return out;
  }
};
} // namespace

std::unique_ptr<RocketModel> makeEssentModel() {
  return std::make_unique<EssentRocketModel>();
}
