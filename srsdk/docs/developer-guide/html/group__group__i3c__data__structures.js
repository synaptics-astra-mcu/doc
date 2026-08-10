var group__group__i3c__data__structures =
[
    [ "i3c_device_config_t", "structi3c__device__config__t.html", [
      [ "device_type", "structi3c__device__config__t.html#a1394dca8a948774e3ce8c9a633794df3", null ],
      [ "static_address", "structi3c__device__config__t.html#af282475a100ab30a2aa86275c879ca6b", null ],
      [ "dynamic_address", "structi3c__device__config__t.html#aed2f2b7a57da8c4715960b8c05c18774", null ],
      [ "nack_retry_count", "structi3c__device__config__t.html#aa32b7041fafe893dbe298d87c74192fd", null ],
      [ "accept_ibi", "structi3c__device__config__t.html#a9c6315c713e8670bfe2c052730a15c53", null ],
      [ "ibi_has_payload", "structi3c__device__config__t.html#a8dc51df3cbd8680abe3fb26ebd64aa91", null ],
      [ "enable_ibi_pec", "structi3c__device__config__t.html#a3eb05e533102380a990f7da616bbd153", null ]
    ] ],
    [ "i3c_target_id_t", "structi3c__target__id__t.html", [
      [ "bcr", "structi3c__target__id__t.html#a5a7c640f3f167906c5a274ed82f39b36", null ],
      [ "dcr", "structi3c__target__id__t.html#afd599066830176585807cec937eb04c7", null ],
      [ "pid", "structi3c__target__id__t.html#a40d9a8c2ba4621e4d5065ee12f85eee1", null ]
    ] ],
    [ "i3c_device_t", "structi3c__device__t.html", [
      [ "device_type", "structi3c__device__t.html#adeb3f7d4f6e31a2adbb39d8c3096b8bf", null ],
      [ "target_id", "structi3c__device__t.html#a2c8f371a497ae86b48a88a8663dfe8cc", null ],
      [ "static_address", "structi3c__device__t.html#ab930c130188ff908eebe575b74468b2d", null ],
      [ "dynamic_address", "structi3c__device__t.html#ab0c3b79e2404023bc8d3a54ad4992b26", null ],
      [ "dat_index", "structi3c__device__t.html#a364cb0391f0da155b6d6199c1ad12052", null ],
      [ "accept_ibi", "structi3c__device__t.html#a5f0615e0c154592d621bfff877a50ce1", null ],
      [ "ibi_has_payload", "structi3c__device__t.html#a19ae3096002c738f4222e3406d52813e", null ],
      [ "enable_ibi_pec", "structi3c__device__t.html#a2b4ceee1237a6302768fa132eba1ab90", null ]
    ] ],
    [ "i3c_dat_entry_t", "structi3c__dat__entry__t.html", [
      [ "device_type", "structi3c__dat__entry__t.html#a88bc84f262cde7bbec5f0a5f196a0ed8", null ],
      [ "static_address", "structi3c__dat__entry__t.html#a6f1d7bfc5563939ad5e7ae91ef587576", null ],
      [ "dynamic_address", "structi3c__dat__entry__t.html#a1eaba5c61ee5b3477e63622c11cc8434", null ],
      [ "nack_retry_count", "structi3c__dat__entry__t.html#a4f77b55dc08cca79acc3a5ac8c55456d", null ],
      [ "accept_ibi", "structi3c__dat__entry__t.html#a0ffcc6f2440cc642b4662a0f55afcb0c", null ],
      [ "ibi_has_payload", "structi3c__dat__entry__t.html#a568ab0c9ea134c03019cb7458235b8b5", null ],
      [ "enable_ibi_pec", "structi3c__dat__entry__t.html#a24c0712a064d0e528294f6a9ce3e4f12", null ]
    ] ],
    [ "i3c_current_xfer_t", "structi3c__current__xfer__t.html", [
      [ "transfer_status", "structi3c__current__xfer__t.html#aafb2d37b9cd574ae2b74fa7637ef1f1f", null ],
      [ "xfer_speed", "structi3c__current__xfer__t.html#ac348687d566c3830dcb45355acda9bb5", null ],
      [ "dat_index", "structi3c__current__xfer__t.html#aac9d360a194250720a6301445e6516be", null ]
    ] ],
    [ "i3c_ccc_response_entry_t", "structi3c__ccc__response__entry__t.html", [
      [ "dat_index", "structi3c__ccc__response__entry__t.html#a2e84322c4a8c7b6ae1deb40da0d11b16", null ],
      [ "ccc_id", "structi3c__ccc__response__entry__t.html#a0bab54e87698f92c6c02c4b8e47ada82", null ],
      [ "status", "structi3c__ccc__response__entry__t.html#ae2983002cc9d885dfbfa85c805d0e63c", null ],
      [ "payload_length", "structi3c__ccc__response__entry__t.html#af9cb869bc43e63a89c14914b84def67c", null ]
    ] ],
    [ "i3c_event_info_t", "structi3c__event__info__t.html", [
      [ "event_type", "structi3c__event__info__t.html#a5579f5db37b5f700583378d987f617bd", null ],
      [ "status", "structi3c__event__info__t.html#a2d7fbf7d56b3c6a81d44181e3d74bb3c", null ],
      [ "target_addr", "structi3c__event__info__t.html#a580ff1eb87a9717fc6015681fbd7bbc0", null ],
      [ "dat_index", "structi3c__event__info__t.html#a9648658c589cda7c27f07ad2fb50ae37", null ],
      [ "transferred_length", "structi3c__event__info__t.html#abcf85124c014478b34ebd578d986455e", null ],
      [ "flags", "structi3c__event__info__t.html#afe25b80a77e8364d08f4e79662340d8d", null ]
    ] ],
    [ "queue_thld_t", "structqueue__thld__t.html", [
      [ "ibi_status_thld", "structqueue__thld__t.html#ae4a283c3c10fecf80be16c79df6a1764", null ],
      [ "ibi_data_thld", "structqueue__thld__t.html#a8388f7df72662adb094502e3b7db3c01", null ],
      [ "resp_buf_thld", "structqueue__thld__t.html#ab24d59a343ef7c5e0ae552ee1c173418", null ],
      [ "cmd_empty_buf_thld", "structqueue__thld__t.html#a6587cbf0412d094695dcd84e385609f6", null ]
    ] ],
    [ "data_buffer_thld_t", "structdata__buffer__thld__t.html", [
      [ "rx_start_thld", "structdata__buffer__thld__t.html#a5e1ef1bfc72e4f32be621ebf412e1c94", null ],
      [ "tx_start_thld", "structdata__buffer__thld__t.html#a9972a82f97946dc479a41be3c61e463c", null ],
      [ "rx_buf_thld", "structdata__buffer__thld__t.html#a5184a79be6232f88240ccf42c6e9c001", null ],
      [ "tx_empty_buf_thld", "structdata__buffer__thld__t.html#a62a535ad02e3719941afbed9a8430bd2", null ]
    ] ],
    [ "i3c_threshold_ctrl_t", "structi3c__threshold__ctrl__t.html", [
      [ "queue_thld", "structi3c__threshold__ctrl__t.html#add406d54bb7c845152067c61363cef98", null ],
      [ "data_buffer_thld", "structi3c__threshold__ctrl__t.html#afb45773bd65138eefd960140825b44b1", null ]
    ] ],
    [ "scl_open_drain_timing_t", "structscl__open__drain__timing__t.html", [
      [ "od_high_ns", "structscl__open__drain__timing__t.html#aa11045fc58fd19d5de0c0cb5d30cea3a", null ],
      [ "od_low_ns", "structscl__open__drain__timing__t.html#ad03624919095e11dd02e4ef1a220a747", null ]
    ] ],
    [ "scl_push_pull_timing_t", "structscl__push__pull__timing__t.html", [
      [ "pp_high_ns", "structscl__push__pull__timing__t.html#a65cd798edcd5b04f2b759ecaa4c3f570", null ],
      [ "pp_low_ns", "structscl__push__pull__timing__t.html#ac53f84bf509a8704a8003775f059d3ad", null ]
    ] ],
    [ "scl_fast_mode_timing_t", "structscl__fast__mode__timing__t.html", [
      [ "fm_high_ns", "structscl__fast__mode__timing__t.html#aa6060d9add8ae7315e8f139f922fb712", null ],
      [ "fm_low_ns", "structscl__fast__mode__timing__t.html#a78cbf79409c322a05a7f15116c1b459b", null ]
    ] ],
    [ "scl_fast_mode_plus_timing_t", "structscl__fast__mode__plus__timing__t.html", [
      [ "fmp_high_ns", "structscl__fast__mode__plus__timing__t.html#a6ca6d73fb98d9792dad306d22d34170a", null ],
      [ "fmp_low_ns", "structscl__fast__mode__plus__timing__t.html#a98520d33d59eb77205ec124b6f2fca92", null ]
    ] ],
    [ "scl_ext_low_count_timing_t", "structscl__ext__low__count__timing__t.html", [
      [ "ext_low_ns_4", "structscl__ext__low__count__timing__t.html#a83ef960dd73b1f8b3ded0de133aa9dc3", null ],
      [ "ext_low_ns_3", "structscl__ext__low__count__timing__t.html#a665d21a4b806c37bc696429e0f5d5119", null ],
      [ "ext_low_ns_2", "structscl__ext__low__count__timing__t.html#a77afc98813befcaebbcec623801ff4eb", null ],
      [ "ext_low_ns_1", "structscl__ext__low__count__timing__t.html#a08b35e747964a74fa806af26a1ab3ce5", null ]
    ] ],
    [ "i3c_bus_avail_timing_t", "structi3c__bus__avail__timing__t.html", [
      [ "bus_free_time_mixed_ns", "structi3c__bus__avail__timing__t.html#a460a3a94dce265d61397021f600ff3f2", null ],
      [ "bus_free_time_pure_ns", "structi3c__bus__avail__timing__t.html#a0fa72ed5629b4221404cbb829758693f", null ]
    ] ],
    [ "i3c_scl_timing_t", "structi3c__scl__timing__t.html", [
      [ "open_drain_timing", "structi3c__scl__timing__t.html#a376b4da9e1fabe0b442ebcc8bbe8482e", null ],
      [ "push_pull_timing", "structi3c__scl__timing__t.html#a9ea179f5f364ff99bee0fecdb1946201", null ],
      [ "fast_mode_timing", "structi3c__scl__timing__t.html#a773bee6148a898e160bc5f9ec35b4629", null ],
      [ "fast_mode_plus_timing", "structi3c__scl__timing__t.html#a7a48900466fc6f5ef6472128a967dcdf", null ],
      [ "ext_low_count_timing", "structi3c__scl__timing__t.html#a9fe091c41740e66e1c8b4a041d954edc", null ]
    ] ],
    [ "i3c_hdr_config_t", "structi3c__hdr__config__t.html", [
      [ "mode", "structi3c__hdr__config__t.html#aa46dc166d90d2bd1287614a6a3e02d37", null ],
      [ "enable_cmd", "structi3c__hdr__config__t.html#ac47908f7821b99d82c264a6923ae022e", null ],
      [ "enable_data", "structi3c__hdr__config__t.html#a67472766e14f10d9f93efc829f2f81b4", null ]
    ] ],
    [ "i3c_interrupt_config_t", "structi3c__interrupt__config__t.html", [
      [ "event_mask", "structi3c__interrupt__config__t.html#ad5e358c685b6cfdaffe17f6204901137", null ],
      [ "notify_sir_rejected", "structi3c__interrupt__config__t.html#ab58dde3165549e0aef5387e9b972e2c0", null ],
      [ "notify_mr_rejected", "structi3c__interrupt__config__t.html#a8a023e257f9464cdbd54585a8c901d83", null ],
      [ "notify_hj_rejected", "structi3c__interrupt__config__t.html#aa832e61aaa06ffce5199b85170031690", null ]
    ] ],
    [ "i3c_controller_policy_config_t", "structi3c__controller__policy__config__t.html", [
      [ "enable_hot_join", "structi3c__controller__policy__config__t.html#a67a23f9f7d21e3dbb52306a676ddf420", null ],
      [ "iba_include", "structi3c__controller__policy__config__t.html#a6f7550b082ad2597ff7776bb4605399c", null ]
    ] ],
    [ "i3c_controller_config_t", "structi3c__controller__config__t.html", [
      [ "slave_bus_idle_time_ns", "structi3c__controller__config__t.html#a34f2d6bf8950d48e4f4331dc230836c2", null ],
      [ "op_mode", "structi3c__controller__config__t.html#aec59106199af353ad2575c0cac98519d", null ],
      [ "threshold_ctrl", "structi3c__controller__config__t.html#ada3f34c6ca15993614da388a613d4731", null ],
      [ "sdr_timing_preset", "structi3c__controller__config__t.html#a6441ebe31f4aeb768572396d02e1c6c6", null ],
      [ "scl_timing", "structi3c__controller__config__t.html#a37146cbb910c0b9dda6f0b09bc7860c5", null ],
      [ "bus_avail_timing", "structi3c__controller__config__t.html#af0489e3c6f5d65ff76f44d2db1b08387", null ],
      [ "slave_bus_available_time_ns", "structi3c__controller__config__t.html#abc9b5c1cf01e3334eb0894f37b627fdb", null ],
      [ "self_static_addr", "structi3c__controller__config__t.html#ab98d47ba3453df50f80e3ac530f97b0c", null ],
      [ "self_dynamic_addr", "structi3c__controller__config__t.html#a05939f0a52d5d4ac46a5a6733f3e483c", null ],
      [ "num_of_devices", "structi3c__controller__config__t.html#a620c3d1c2597b9b643cafe47ff693a4a", null ],
      [ "i2c_target_present", "structi3c__controller__config__t.html#a3a3508c645f6723d33e26e60299d823f", null ]
    ] ],
    [ "i3c_target_config_t", "structi3c__target__config__t.html", [
      [ "bus_idle_time_ns", "structi3c__target__config__t.html#a7e3fb08ffa3b29dddeed7d9240c74e25", null ],
      [ "op_mode", "structi3c__target__config__t.html#ada8e78047fab28bf749dd9ce3690ad39", null ],
      [ "target_type", "structi3c__target__config__t.html#a2f4e778b3ec9936d1cf51af80d464e5d", null ],
      [ "threshold_ctrl", "structi3c__target__config__t.html#ac9133d3bd6c298c1bc2781ae8ba93bc3", null ],
      [ "self_device", "structi3c__target__config__t.html#a779df58a5e7a2d25abcc422783b8717b", null ],
      [ "bus_available_time_ns", "structi3c__target__config__t.html#ac3c54211f32327567fec3bfeb6388af3", null ],
      [ "self_static_addr", "structi3c__target__config__t.html#a2b689c1393fd189465252d4ced3fbc21", null ],
      [ "idle_cnt_multiplier", "structi3c__target__config__t.html#ac359c5c2372658213b32478f0c20a593", null ],
      [ "clk_data_turn", "structi3c__target__config__t.html#ac06b0cc5e16453c0d92287e62ced122d", null ],
      [ "max_read_speed", "structi3c__target__config__t.html#a90549691dc718d27960dfa2126f2efee", null ],
      [ "max_write_speed", "structi3c__target__config__t.html#a0dd87465148a8723672b7cf997823fd9", null ],
      [ "is_adaptive_i2c", "structi3c__target__config__t.html#ad76cfa202320dbfe8b5c475f2c6ab4af", null ]
    ] ],
    [ "i3c_transfer_t", "structi3c__transfer__t.html", [
      [ "addr_mode", "structi3c__transfer__t.html#a0a69ac1c5103dc4694e39fb7ebc9df27", null ],
      [ "address", "structi3c__transfer__t.html#a67bb5a9a0e55baa3f64e314ca37a23b4", null ],
      [ "dat_index", "structi3c__transfer__t.html#a7915b3cce0c602edebe629f765d3a56c", null ],
      [ "buf", "structi3c__transfer__t.html#a31226183958a4103c4fd9f15f4259509", null ],
      [ "size", "structi3c__transfer__t.html#ae776464bf3c5b110c7753e649cde149e", null ],
      [ "timeout", "structi3c__transfer__t.html#a70dda51de4a27e44c114d67a4d6672da", null ],
      [ "speed", "structi3c__transfer__t.html#a30f73d617f199187f96a92e88d07fc1d", null ],
      [ "direction", "structi3c__transfer__t.html#aba00f42a520f6269e5de03df619b2667", null ],
      [ "flags", "structi3c__transfer__t.html#ad1bf230b9dedca6fa3b57d752156ada9", null ]
    ] ],
    [ "i3c_ibi_config_t", "structi3c__ibi__config__t.html", [
      [ "set_reject", "structi3c__ibi__config__t.html#afb9318a79782032aed28b979c5caa836", null ],
      [ "is_ibi_with_data", "structi3c__ibi__config__t.html#adce305f535f1e5faf894f357c751c40b", null ]
    ] ],
    [ "i3c_sir_config_t", "structi3c__sir__config__t.html", [
      [ "buf", "structi3c__sir__config__t.html#a6090f25e0f7d35fc2ac98db472b4db97", null ],
      [ "size", "structi3c__sir__config__t.html#ab4ce4c545008cf4a2ec17aeb2bf79cd6", null ]
    ] ],
    [ "i3c_event_cb_t", "group__group__i3c__data__structures.html#ga78aaf028125a43a5a1aed215932ec916", null ]
];