#ifndef JAMLAB_H
#define JAMLAB_H

static struct txdata data = {
  .dummy = {
    0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, // SA
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, // BSSID
    0x00, 0x00,
    0x00, 0x11, 0x22, 0x33, 0x44, 0x51, // BSSID
    0x00, 0x00, 0x27, 0x00, 0x2b, 0x40, 0x08, 0xa0, 0x20, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xc9, 0x8b, 0x35, 0x2c, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x9e, 0x09, 0x80, 0x04, 0xa9, 0x00,
    0x00, 0x00, 0x07, 0x00, 0x00, 0xa9, 0x00, 0x88, 0x4a, 0x3c, 0x00, 0x5c, 0x93, 0xa2, 0xe8, 0x96,
    0x0b, 0xa0, 0xcf, 0x5b, 0xa3, 0x17, 0xb1, 0xa2, 0x01, 0x00, 0x00, 0x00, 0x20, 0x20, 0x56, 0x00,
    0x00, 0x08, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x85, 0x63, 0x4c, 0x80, 0xb7, 0xe0, 0x17,
    0xd3, 0xcc, 0x88, 0x9f, 0x39, 0x73, 0x65, 0xbf, 0x1d, 0xff, 0xad, 0x63, 0x6a, 0xdf, 0x8f, 0x72,
    0xaa, 0x53, 0x74, 0x07, 0x26, 0x0b, 0xc4, 0xf7, 0xf7, 0x8b, 0x73, 0x11, 0x59, 0x1b, 0x35, 0xcc,
    0xea, 0x9b, 0x13, 0xb5, 0x5e, 0xe2, 0x4b, 0x2f, 0x05, 0x23, 0x1f, 0xc0, 0x58, 0xee, 0x24, 0xcd,
    0xdb, 0x4e, 0x61, 0x0b, 0xee, 0xd2, 0x33, 0xfd, 0xbe, 0x39, 0x11, 0x85, 0x64, 0x17, 0xc8, 0x86,
    0xd5, 0xee, 0x5c, 0xb5, 0xf1, 0x4d, 0xc1, 0xf2, 0x3a, 0x18, 0x1b, 0x4d, 0x19, 0x92, 0x93, 0xed,
    0x15, 0xf8, 0xc8, 0xf6, 0x6d, 0x14, 0x65, 0x63, 0x19, 0xc6, 0x13, 0xb6, 0x94, 0x9c, 0x3e, 0x6d,
    0x64, 0xa4, 0x5e, 0x6d, 0xc8, 0x25, 0xcb, 0x01, 0x85, 0x9a, 0x67, 0x34, 0x07, 0x72, 0x7a, 0x6b,
    0xf1, 0x52, 0x70, 0x2d, 0x1a, 0x4e, 0xcd, 0x2f, 0x70, 0xea, 0x81, 0x86, 0x28, 0x87, 0x26, 0x5b,
    0x7b, 0x60, 0xe4, 0x13, 0xc6, 0xf6, 0x33, 0x59, 0xb3, 0xbd, 0x71, 0xdf, 0xb5, 0x2d, 0xe6, 0x34,
    0x8b, 0x24, 0xa3, 0xd7, 0xa6, 0x8d, 0xe1, 0xf7, 0x0e, 0xee, 0xda, 0x38, 0xf3, 0xdb, 0x78, 0xb1,
    0xc0, 0x02, 0x31, 0xed, 0x73, 0x65, 0x13, 0xe6, 0x7a, 0x85, 0xc5, 0x29, 0x08, 0x95, 0xb3, 0x2e,
    0xdc, 0x2f, 0x3c, 0xbb, 0xa8, 0x3b, 0xc9, 0x4f, 0xe1, 0x0f, 0xb6, 0xc8, 0x4b, 0x02, 0x18, 0x96,
    0x43, 0x07, 0x17, 0xed, 0x1d, 0x15, 0x2f, 0xdb, 0x78, 0x55, 0xa6, 0x64, 0x1f, 0x43, 0x86, 0xed,
    0xb7, 0x82, 0xfe, 0x6a, 0x3c, 0xb8, 0xe5, 0x6e, 0xbb, 0x91, 0x2c, 0x37, 0xb6, 0xd0, 0x4a, 0x7a,
    0x2c, 0x69, 0x45, 0x48, 0x2b, 0xf4, 0x9a, 0x51, 0xac, 0x00, 0xe0, 0xb9, 0x1f, 0xa4, 0x84, 0x24,
    0x93, 0x12, 0x3c, 0xa1, 0x9a, 0x06, 0x26, 0x0e, 0x76, 0xad, 0x5e, 0x18, 0x80, 0x30, 0x39, 0x7b,
    0xd1, 0x23, 0x96, 0x05, 0xd4, 0xa5, 0x58, 0x75, 0xa0, 0x45, 0xdb, 0x2d, 0x11, 0x33, 0x41, 0xb9,
    0xbd, 0x57, 0x17, 0x3e, 0x92, 0x5b, 0xeb, 0xb5, 0xb2, 0x08, 0x50, 0xe1, 0x53, 0x6b, 0x73, 0xc6,
    0x94, 0x1b, 0x7c, 0x61, 0x7f, 0xa2, 0xca, 0x2f, 0x6d, 0x43, 0x70, 0x1b, 0xe1, 0xe1, 0x76, 0x13,
    0x6f, 0x9f, 0x22, 0x80, 0xcb, 0x87, 0x4a, 0x5a, 0xb9, 0x4d, 0x89, 0xc2, 0xca, 0x5c, 0x78, 0x2f,
    0x22, 0xdb, 0x44, 0xec, 0x17, 0x6a, 0x1b, 0xe4, 0x4d, 0x52, 0x08, 0x0d, 0xdb, 0x7b, 0x75, 0x6b,
    0x92, 0x0a, 0x41, 0x5d, 0x56, 0x9c, 0x59, 0x3d, 0xb6, 0xac, 0xe3, 0x08, 0xe4, 0xac, 0xa8, 0x5e,
    0xb9, 0xcd, 0xe6, 0x2b, 0x96, 0x11, 0x93, 0x0e, 0xe4, 0xcd, 0x3f, 0xe4, 0xe8, 0xad, 0xb4, 0xb9,
    0x84, 0xdd, 0x7b, 0x8b, 0x55, 0x9d, 0x87, 0x47, 0x72, 0xbb, 0x7a, 0xb4, 0xb7, 0x5a, 0x17, 0x02,
    0x4f, 0xcc, 0x99, 0xf6, 0xa0, 0xda, 0xfb, 0xd9, 0xdd, 0x8e, 0xf8, 0x87, 0x6b, 0xde, 0x1e, 0xff,
    0x08, 0x74, 0x0a, 0xf2, 0xa4, 0x79, 0xa8, 0xdc, 0xe7, 0x07, 0x99, 0x72, 0xc3, 0xf7, 0x3e, 0xbd,
    0x5d, 0xcc, 0x98, 0xa9, 0x6e, 0xe6, 0x6e, 0x85, 0xb0, 0x41, 0xb1, 0x90, 0x74, 0x45, 0xb0, 0x07,
    0xeb, 0x72, 0xe4, 0x85, 0x9c, 0x38, 0xe3, 0x7b, 0x9c, 0xc4, 0x1c, 0x40, 0x53, 0x44, 0x19, 0xea,
    0x79, 0xb0, 0x59, 0x2a, 0xed, 0x3f, 0xb7, 0x5b, 0xb8, 0xcf, 0xb7, 0xa2, 0x92, 0xee, 0x47, 0x6c,
    0x07, 0x60, 0x23, 0xf2, 0xe3, 0xc4, 0xde, 0x81, 0x20, 0x22, 0x82, 0x3f, 0xd1, 0x99, 0xc8, 0x21,
    0x19, 0xb1, 0x46, 0xee, 0xf9, 0xc1, 0x12, 0x03, 0xf2, 0xdc, 0x45, 0x1a, 0x34, 0xde, 0x62, 0xbb,
    0x82, 0xdf, 0xa4, 0x3e, 0xf5, 0xcf, 0xa8, 0x83, 0xa5, 0x32, 0xd1, 0xed, 0xd8, 0x03, 0xa9, 0x23,
    0xfb, 0xb7, 0x40, 0xad, 0xde, 0x97, 0xcb, 0x73, 0xc5, 0x89, 0x14, 0xd8, 0x61, 0x97, 0x8b, 0xeb,
    0x4f, 0x9a, 0x93, 0x11, 0x74, 0x74, 0x5d, 0x3f, 0xbc, 0x16, 0xac, 0xdb, 0x14, 0x00, 0x3a, 0x57,
    0x18, 0xb5, 0xa0, 0xb6, 0x72, 0x59, 0xd4, 0x7e, 0xb8, 0xde, 0x33, 0x8b, 0x00, 0xc3, 0x4a, 0x9c,
    0x67, 0x79, 0xa7, 0xec, 0x22, 0xe8, 0x6e, 0x5f, 0x87, 0x32, 0x5c, 0x36, 0xe0, 0xec, 0xa9, 0xaa,
    0xdd, 0x47, 0xae, 0x7b, 0x2e, 0x87, 0x47, 0xce, 0xd7, 0xb8, 0x94, 0xf2, 0xa2, 0xcb, 0x1f, 0x01,
    0xa0, 0xdb, 0x78, 0x24, 0xdb, 0x56, 0xb6, 0x65, 0xc8, 0xa0, 0x91, 0x16, 0x61, 0x09, 0xfd, 0xcc,
    0xe8, 0xa3, 0xfa, 0x2a, 0x78, 0x12, 0xf8, 0xb4, 0x2a, 0x40, 0xd4, 0xb2, 0x28, 0xf4, 0xc0, 0xa5,
    0x54, 0x28, 0x61, 0xc6, 0x0f, 0xdf, 0x33, 0x57, 0x47, 0x7a, 0xab, 0x88, 0xb1, 0x71, 0x4b, 0xd3,
    0xe5, 0x09, 0x04, 0xa3, 0x5b, 0x96, 0x4f, 0x4b, 0x74, 0x35, 0xbb, 0x12, 0x0e, 0x95, 0x58, 0x79,
    0x69, 0x07, 0x74, 0x54, 0x1a, 0x1e, 0xea, 0xb1, 0xc5, 0x1a, 0x04, 0x29, 0x9c, 0xe7, 0x90, 0x62,
    0xec, 0x3b, 0x04, 0x97, 0x63, 0xf9, 0xf5, 0xa4, 0xac, 0x49, 0x51, 0x1e, 0x49, 0x28, 0x23, 0x49,
    0x58, 0x5a, 0xe2, 0x21, 0x3d, 0x60, 0xca, 0x72, 0x71, 0x52, 0x77, 0x06, 0x62, 0x3c, 0x9c, 0x81,
    0x74, 0xc9, 0x36, 0x50, 0x3c, 0xa7, 0x0f, 0xc0, 0x11, 0x1e, 0x55, 0x3e, 0x08, 0x9d, 0x92, 0xd6,
    0xf5, 0x3d, 0x1d, 0x3e, 0xb6, 0xb8, 0xd7, 0x34, 0xec, 0x18, 0xb1, 0x4b, 0x09, 0xb3, 0x59, 0xa0,
    0x3b, 0xfc, 0x5d, 0x73, 0x85, 0xd4, 0xb2, 0x74, 0x10, 0x55, 0xb9, 0x1e, 0x96, 0xbc, 0x4e, 0x5f,
    0x94, 0x2a, 0x61, 0x62, 0x01, 0xe3, 0xd8, 0x7e, 0xe8, 0x76, 0x2b, 0x89, 0xb1, 0x84, 0x17, 0x9c,
    0x9a, 0xe9, 0x62, 0x87, 0x60, 0xd5, 0xc5, 0x7d, 0x95, 0xe4, 0x32, 0xb2, 0x00, 0x79, 0x2a, 0xe9,
    0x66, 0x74, 0x14, 0xf6, 0x94, 0x07, 0x79, 0x01, 0x60, 0x79, 0x94, 0x47, 0x3b, 0x42, 0x65, 0x2e,
    0x57, 0xca, 0xa7, 0xd5, 0xa1, 0xd4, 0x8a, 0x07, 0x5f, 0xf2, 0xa7, 0x67, 0xb6, 0x83, 0xdb, 0x3b,
    0xc5, 0xa2, 0x39, 0x6b, 0x41, 0x6e, 0x88, 0xed, 0xf0, 0xf1, 0xf8, 0x4c, 0x4a, 0x58, 0x63, 0x10,
    0xa2, 0xef, 0x7d, 0xd4, 0x20, 0xd9, 0x26, 0xda, 0xac, 0x0f, 0x80, 0x22, 0x46, 0x2d, 0xd7, 0x65,
    0xe2, 0x3b, 0x94, 0xa5, 0x51, 0xdc, 0x97, 0x18, 0x15, 0xd5, 0xdd, 0x4a, 0x61, 0x17, 0x90, 0xb6,
    0x15, 0x8c, 0x8b, 0xd5, 0x53, 0xcc, 0x53, 0xaf, 0x42, 0x58, 0xec, 0x25, 0x94, 0xca, 0x57, 0x3c,
    0xbb, 0xaa, 0x83, 0x0f, 0xa7, 0x2f, 0xfd, 0x34, 0x80, 0xf8, 0x4c, 0xfd, 0xa6, 0x53, 0x1c, 0x57,
    0x03, 0x49, 0x05, 0x2f, 0x63, 0x94, 0x34, 0xc7, 0x0d, 0xbe, 0x0d, 0x00, 0x4f, 0x98, 0x4e, 0xdd,
    0xe4, 0xfd, 0x1e, 0x0b, 0xef, 0x55, 0x41, 0xab, 0x6f, 0x4e, 0xe8, 0x07, 0xb4, 0x0c, 0x01, 0xc2,
    0x86, 0x50, 0xb0, 0x42, 0x06, 0xbd, 0x48, 0x60, 0x22, 0xc4, 0xae, 0xa4, 0x95, 0x1a, 0xcc, 0x4d,
    0x89, 0x96, 0x0f, 0x11, 0x93, 0x09, 0x93, 0x36, 0x20, 0x84, 0x91, 0x08, 0x42, 0x8a, 0x48, 0x42,
    0x17, 0x42, 0xa7, 0xb7, 0xa2, 0xc2, 0x59, 0xe7, 0x88, 0xc2, 0x9f, 0x1e, 0x2e, 0x9e, 0x4e, 0x4f,
    0xe5, 0x43, 0x49, 0xa9, 0x5f, 0xef, 0xed, 0x7b, 0x80, 0xa2, 0x1f, 0xa5, 0x28, 0xbc, 0x1b, 0x2b,
    0xe6, 0xda, 0x95, 0xc8, 0x5b, 0xee, 0x00, 0x1e, 0x1c, 0x35, 0xce, 0x82, 0xf5, 0x1c, 0x7f, 0x66,
    0xd5, 0x7c, 0x70, 0xfc, 0x51, 0x9e, 0xeb, 0x20, 0x5b, 0xa2, 0xb5, 0x0a, 0x1e, 0x21, 0x9c, 0x62,
    0xcf, 0xaa, 0xa6, 0x0c, 0x43, 0x89, 0xb5, 0x50, 0x9a, 0x82, 0x38, 0x1a, 0x82, 0x2d, 0x3a, 0x08,
    0xfb, 0x10, 0xf1, 0x83, 0x0e, 0x3f, 0xe3, 0xba, 0x10, 0x0e, 0x76, 0x50, 0x63, 0x06, 0x9a, 0xce,
    0x41, 0x6d, 0xf0, 0xfd, 0xad, 0x16, 0xec, 0x8d, 0xf3, 0x46, 0x64, 0x22, 0xe1, 0x66, 0xa7, 0x6a,
    0x31, 0x05, 0xae, 0xf8, 0x3b, 0xa3, 0xa1, 0xda, 0x0e, 0x74, 0x0d, 0xb9, 0xc0, 0xe8, 0xb0, 0x88,
    0xd9, 0xc8, 0x99, 0x20, 0xff, 0xe7, 0xc9, 0xf5, 0xd6, 0x7a, 0xbb, 0xef, 0x00, 0xb6, 0x21, 0xea,
    0x3a, 0x11, 0x4e, 0x91, 0x4a, 0xe1, 0x5f, 0x86, 0x61, 0x74, 0x5a, 0x15, 0xd9, 0x8e, 0x70, 0x49,
    0xf2, 0x48, 0x09, 0xcb, 0x80, 0x33, 0x1d, 0x57, 0xaf, 0x5c, 0xeb, 0xe7, 0x5f, 0xcc, 0x71, 0xe3,
    0x1d, 0x97, 0xf5, 0x70, 0x82, 0xf3, 0x94, 0xcb, 0x2a, 0xbf, 0x2e, 0xbb, 0x93, 0xd9, 0x3d, 0x1b,
    0x9e, 0x7c, 0x2e, 0xbb, 0x0e, 0x6c, 0x2e, 0xef, 0x6a, 0x10, 0x81, 0x13, 0xbd, 0x06, 0x95, 0x6a,
    0xa1, 0xe7, 0xaf, 0x7a, 0x86, 0xc9, 0x9d, 0xed, 0xd9, 0x79, 0xf5, 0x55, 0x6a, 0x0c, 0xb3, 0x2d,
    0x9b, 0xf5, 0x5a, 0x06, 0x73, 0x42, 0xf1, 0x20, 0xaa, 0x24, 0x2a, 0x21, 0x45, 0x4b, 0x4d, 0xc9,
    0x4b, 0xa5, 0x34, 0x07, 0x87, 0xfe, 0x35, 0x05, 0xcb, 0x58, 0xec, 0xc9, 0xb5, 0xb1, 0xed, 0x6f,
    0x2e, 0xb3, 0x9d, 0x5b, 0xb9, 0x19, 0xb2, 0x1f, 0x45, 0x80, 0x0c, 0x40, 0x8d, 0x43, 0x44, 0xb2,
    0x02, 0xc8, 0xcf, 0xdd, 0x30, 0x18, 0x2f, 0xa9, 0x25, 0x66, 0x2a, 0x1e, 0x67, 0x67, 0x14, 0xb8,
    0x41, 0x2b, 0xb9, 0x4e, 0x53, 0x15, 0x1f, 0xe0, 0x6c, 0xb2, 0xa7, 0xdc, 0x23, 0xa6, 0xdc, 0xab,
    0xdf, 0x46, 0xc9, 0x64, 0x71, 0x3c, 0xb3, 0xaf, 0xcf, 0x1c, 0xb7, 0xd8, 0xab, 0x00, 0x36, 0xc2,
    0x58, 0x95, 0xd2, 0xee, 0xcf, 0x4d, 0x28, 0x48, 0xaf, 0x5b, 0x1b, 0x84, 0x82, 0x91, 0xc7, 0xae,
    0xaa, 0xcc, 0xf6, 0x7b, 0xa1, 0x4d, 0x70, 0x5b, 0x68, 0x25, 0x3d, 0x76, 0x30, 0xc0, 0x6c, 0xee,
    0x41, 0xbf, 0x1f, 0x15, 0xcc, 0x35, 0xf1, 0x12, 0x6d, 0x2d, 0x64, 0xa1, 0xf1, 0xad, 0x36, 0xc8,
    0xc6, 0xe6, 0xee, 0x5a, 0x57, 0xc6, 0x21, 0x7a, 0xdf, 0x9b, 0xb3, 0xae, 0x5f, 0xe0, 0xf9, 0x8b,
    0x73, 0x00, 0x95, 0xe1, 0x6d, 0x32, 0xc8, 0x03, 0x8d, 0x92, 0x2d, 0xdc, 0x1e, 0x61, 0xe4, 0xf6,
    0xf5, 0x7a, 0xd4, 0x45, 0x1d, 0xfc, 0x75, 0x51, 0x2f, 0x4c, 0x6b, 0xd3, 0x2c, 0x0e, 0x68, 0x4f,
    0xc3, 0x2d, 0x75, 0xe5, 0x80, 0x52, 0xff, 0x29, 0x7a, 0x16, 0x17, 0x34, 0xf0, 0x6e, 0xd9, 0x5b,
    0xf1, 0x77, 0xad, 0x15, 0x2d, 0x51, 0x2a, 0x82, 0xdf, 0xe3, 0xcf, 0x25, 0x80, 0x13, 0xff, 0xda,
    0x22, 0xc7, 0x6f, 0xe2, 0x99, 0x61, 0xac, 0x08
  }
};

#define MAX_JAM_PACK 1526
#define MIN_JAM_PACK 16 // empirically measured

#define MIN_JAM_COUNT 1
#define MAX_JAM_COUNT 10000 

static void * hwtimer=0;
static unsigned int hwinterval = 13000; //my timer interval
static unsigned char hwstop = 0; //prevent refire

static struct hndrte_timer* timer = 0; //my interval timer
static unsigned int interval = 13; //my timer interval
static unsigned char stop = 0; //prevent refire

static struct hndrte_timer* task_timer = 0; //my interval timer
static unsigned int task_interval = 100; //my timer interval
static unsigned char task_stop = 0; //prevent refire

static unsigned int packet_len = MAX_JAM_PACK; //how much to send
static unsigned int packet_count = MIN_JAM_COUNT; //how often to send

static unsigned int txdelay=100;
static unsigned int transmission_speed = 0; // can only be a specified number (see 802.11g) * 2 aka 54Mbit/s is 108 etc
static unsigned char pwr_db=10;

static unsigned char channel = 1; //channel to send on
static unsigned char deaf = 0; //control csma deaf mode

unsigned short prng_rand();
void prng_seed(unsigned int seed);

#endif /*JAMLAB_H*/
