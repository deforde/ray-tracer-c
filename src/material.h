#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "hit_record.h"
#include "ray.h"
#include "vec.h"

typedef bool (*material_scatter_func_t)(void* object, ray_t r_in, hit_record_t rec, colour_t* attenuation, ray_t* scattered);

typedef struct {
    material_scatter_func_t scatter_func;
} material_vtable_t;

typedef struct material_struct_t {
    material_vtable_t* vtable;
} material_t;

bool material_scatter(material_t* material, ray_t r_in, hit_record_t rec, colour_t* attenuation, ray_t* scattered);

#endif //__MATERIAL_H__
