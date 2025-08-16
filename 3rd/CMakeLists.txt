add_subdirectory(argparse)

# "Package maintainers are encouraged to use at least the following options for the shared library"
# I'll take that as "use these options for the most accurate emulation"
set(Z80_WITH_EXECUTE YES)
set(Z80_WITH_FULL_IM0 YES)
set(Z80_WITH_IM0_RETX_NOTIFICATIONS YES)
set(Z80_WITH_Q YES)
set(Z80_WITH_ZILOG_NMOS_LD_A_IR_BUG YES)
set(Z80_SHARED_LIBS NO)
add_subdirectory(Z80)
