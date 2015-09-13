#ifndef MSM_VFE_READ_H
#define MSM_VFE_READ_H

#include <stdbool.h>

bool msm_vfe_read_value_at_address(unsigned long address, int *value);
bool msm_vfe_read_values_at_address(unsigned long address, int *values, int count);

#endif /* MSM_VFE_READ_H */
/*
vi:ts=2:nowrap:ai:expandtab:sw=2
*/
