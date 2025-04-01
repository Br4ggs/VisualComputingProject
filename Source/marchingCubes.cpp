#include "header/marchingCubes.h"

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

constexpr int edgeTable[256] = {
	0x0, 0x109, 0x203, 0x30a, 0x80c, 0x905, 0xa0f, 0xb06,
	0x406, 0x50f, 0x605, 0x70c, 0xc0a, 0xd03, 0xe09, 0xf00,
	0x190, 0x99, 0x393, 0x29a, 0x99c, 0x895, 0xb9f, 0xa96,
	0x596, 0x49f, 0x795, 0x69c, 0xd9a, 0xc93, 0xf99, 0xe90,
	0x230, 0x339, 0x33, 0x13a, 0xa3c, 0xb35, 0x83f, 0x936,
	0x636, 0x73f, 0x435, 0x53c, 0xe3a, 0xf33, 0xc39, 0xd30,
	0x3a0, 0x2a9, 0x1a3, 0xaa, 0xbac, 0xaa5, 0x9af, 0x8a6,
	0x7a6, 0x6af, 0x5a5, 0x4ac, 0xfaa, 0xea3, 0xda9, 0xca0,
	0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc, 0x1c5, 0x2cf, 0x3c6,
	0xcc6, 0xdcf, 0xec5, 0xfcc, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
	0x950, 0x859, 0xb53, 0xa5a, 0x15c, 0x55, 0x35f, 0x256,
	0xd56, 0xc5f, 0xf55, 0xe5c, 0x55a, 0x453, 0x759, 0x650,
	0xaf0, 0xbf9, 0x8f3, 0x9fa, 0x2fc, 0x3f5, 0xff, 0x1f6,
	0xef6, 0xfff, 0xcf5, 0xdfc, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
	0xb60, 0xa69, 0x963, 0x86a, 0x36c, 0x265, 0x16f, 0x66,
	0xf66, 0xe6f, 0xd65, 0xc6c, 0x76a, 0x663, 0x569, 0x460,
	0x460, 0x569, 0x663, 0x76a, 0xc6c, 0xd65, 0xe6f, 0xf66,
	0x66, 0x16f, 0x265, 0x36c, 0x86a, 0x963, 0xa69, 0xb60,
	0x5f0, 0x4f9, 0x7f3, 0x6fa, 0xdfc, 0xcf5, 0xfff, 0xef6,
	0x1f6, 0xff, 0x3f5, 0x2fc, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
	0x650, 0x759, 0x453, 0x55a, 0xe5c, 0xf55, 0xc5f, 0xd56,
	0x256, 0x35f, 0x55, 0x15c, 0xa5a, 0xb53, 0x859, 0x950,
	0x7c0, 0x6c9, 0x5c3, 0x4ca, 0xfcc, 0xec5, 0xdcf, 0xcc6,
	0x3c6, 0x2cf, 0x1c5, 0xcc, 0xbca, 0xac3, 0x9c9, 0x8c0,
	0xca0, 0xda9, 0xea3, 0xfaa, 0x4ac, 0x5a5, 0x6af, 0x7a6,
	0x8a6, 0x9af, 0xaa5, 0xbac, 0xaa, 0x1a3, 0x2a9, 0x3a0,
	0xd30, 0xc39, 0xf33, 0xe3a, 0x53c, 0x435, 0x73f, 0x636,
	0x936, 0x83f, 0xb35, 0xa3c, 0x13a, 0x33, 0x339, 0x230,
	0xe90, 0xf99, 0xc93, 0xd9a, 0x69c, 0x795, 0x49f, 0x596,
	0xa96, 0xb9f, 0x895, 0x99c, 0x29a, 0x393, 0x99, 0x190,
	0xf00, 0xe09, 0xd03, 0xc0a, 0x70c, 0x605, 0x50f, 0x406,
	0xb06, 0xa0f, 0x905, 0x80c, 0x30a, 0x203, 0x109, 0x0};

constexpr int edgeVertexIndices[12][2] = {
	{0, 1},
	{1, 3},
	{3, 2},
	{2, 0},
	{4, 5},
	{5, 7},
	{7, 6},
	{6, 4},
	{0, 4},
	{1, 5},
	{3, 7},
	{2, 6}};

constexpr int triTable[256][16] = {
	{-1}, {0, 3, 8, -1}, {0, 9, 1, -1}, {3, 8, 1, 1, 8, 9, -1}, {2, 11, 3, -1}, {8, 0, 11, 11, 0, 2, -1},
	{3, 2, 11, 1, 0, 9, -1}, {11, 1, 2, 11, 9, 1, 11, 8, 9, -1}, {1, 10, 2, -1}, {0, 3, 8, 2, 1, 10, -1},
	{10, 2, 9, 9, 2, 0, -1}, {8, 2, 3, 8, 10, 2, 8, 9, 10, -1}, {11, 3, 10, 10, 3, 1, -1},
	{10, 0, 1, 10, 8, 0, 10, 11, 8, -1}, {9, 3, 0, 9, 11, 3, 9, 10, 11, -1}, {8, 9, 11, 11, 9, 10, -1}, {4, 8, 7, -1},
	{7, 4, 3, 3, 4, 0, -1}, {4, 8, 7, 0, 9, 1, -1}, {1, 4, 9, 1, 7, 4, 1, 3, 7, -1}, {8, 7, 4, 11, 3, 2, -1},
	{4, 11, 7, 4, 2, 11, 4, 0, 2, -1}, {0, 9, 1, 8, 7, 4, 11, 3, 2, -1}, {7, 4, 11, 11, 4, 2, 2, 4, 9, 2, 9, 1, -1},
	{4, 8, 7, 2, 1, 10, -1}, {7, 4, 3, 3, 4, 0, 10, 2, 1, -1}, {10, 2, 9, 9, 2, 0, 7, 4, 8, -1},
	{10, 2, 3, 10, 3, 4, 3, 7, 4, 9, 10, 4, -1}, {1, 10, 3, 3, 10, 11, 4, 8, 7, -1},
	{10, 11, 1, 11, 7, 4, 1, 11, 4, 1, 4, 0, -1}, {7, 4, 8, 9, 3, 0, 9, 11, 3, 9, 10, 11, -1},
	{7, 4, 11, 4, 9, 11, 9, 10, 11, -1}, {9, 4, 5, -1}, {9, 4, 5, 8, 0, 3, -1}, {4, 5, 0, 0, 5, 1, -1},
	{5, 8, 4, 5, 3, 8, 5, 1, 3, -1}, {9, 4, 5, 11, 3, 2, -1}, {2, 11, 0, 0, 11, 8, 5, 9, 4, -1},
	{4, 5, 0, 0, 5, 1, 11, 3, 2, -1}, {5, 1, 4, 1, 2, 11, 4, 1, 11, 4, 11, 8, -1}, {1, 10, 2, 5, 9, 4, -1},
	{9, 4, 5, 0, 3, 8, 2, 1, 10, -1}, {2, 5, 10, 2, 4, 5, 2, 0, 4, -1}, {10, 2, 5, 5, 2, 4, 4, 2, 3, 4, 3, 8, -1},
	{11, 3, 10, 10, 3, 1, 4, 5, 9, -1}, {4, 5, 9, 10, 0, 1, 10, 8, 0, 10, 11, 8, -1},
	{11, 3, 0, 11, 0, 5, 0, 4, 5, 10, 11, 5, -1}, {4, 5, 8, 5, 10, 8, 10, 11, 8, -1}, {8, 7, 9, 9, 7, 5, -1},
	{3, 9, 0, 3, 5, 9, 3, 7, 5, -1}, {7, 0, 8, 7, 1, 0, 7, 5, 1, -1}, {7, 5, 3, 3, 5, 1, -1},
	{5, 9, 7, 7, 9, 8, 2, 11, 3, -1}, {2, 11, 7, 2, 7, 9, 7, 5, 9, 0, 2, 9, -1},
	{2, 11, 3, 7, 0, 8, 7, 1, 0, 7, 5, 1, -1}, {2, 11, 1, 11, 7, 1, 7, 5, 1, -1}, {8, 7, 9, 9, 7, 5, 2, 1, 10, -1},
	{10, 2, 1, 3, 9, 0, 3, 5, 9, 3, 7, 5, -1}, {7, 5, 8, 5, 10, 2, 8, 5, 2, 8, 2, 0, -1},
	{10, 2, 5, 2, 3, 5, 3, 7, 5, -1}, {8, 7, 5, 8, 5, 9, 11, 3, 10, 3, 1, 10, -1}, {5, 11, 7, 10, 11, 5, 1, 9, 0, -1},
	{11, 5, 10, 7, 5, 11, 8, 3, 0, -1}, {5, 11, 7, 10, 11, 5, -1}, {6, 7, 11, -1}, {7, 11, 6, 3, 8, 0, -1},
	{6, 7, 11, 0, 9, 1, -1}, {9, 1, 8, 8, 1, 3, 6, 7, 11, -1}, {3, 2, 7, 7, 2, 6, -1}, {0, 7, 8, 0, 6, 7, 0, 2, 6, -1},
	{6, 7, 2, 2, 7, 3, 9, 1, 0, -1}, {6, 7, 8, 6, 8, 1, 8, 9, 1, 2, 6, 1, -1}, {11, 6, 7, 10, 2, 1, -1},
	{3, 8, 0, 11, 6, 7, 10, 2, 1, -1}, {0, 9, 2, 2, 9, 10, 7, 11, 6, -1}, {6, 7, 11, 8, 2, 3, 8, 10, 2, 8, 9, 10, -1},
	{7, 10, 6, 7, 1, 10, 7, 3, 1, -1}, {8, 0, 7, 7, 0, 6, 6, 0, 1, 6, 1, 10, -1},
	{7, 3, 6, 3, 0, 9, 6, 3, 9, 6, 9, 10, -1}, {6, 7, 10, 7, 8, 10, 8, 9, 10, -1}, {11, 6, 8, 8, 6, 4, -1},
	{6, 3, 11, 6, 0, 3, 6, 4, 0, -1}, {11, 6, 8, 8, 6, 4, 1, 0, 9, -1}, {1, 3, 9, 3, 11, 6, 9, 3, 6, 9, 6, 4, -1},
	{2, 8, 3, 2, 4, 8, 2, 6, 4, -1}, {4, 0, 6, 6, 0, 2, -1}, {9, 1, 0, 2, 8, 3, 2, 4, 8, 2, 6, 4, -1},
	{9, 1, 4, 1, 2, 4, 2, 6, 4, -1}, {4, 8, 6, 6, 8, 11, 1, 10, 2, -1}, {1, 10, 2, 6, 3, 11, 6, 0, 3, 6, 4, 0, -1},
	{11, 6, 4, 11, 4, 8, 10, 2, 9, 2, 0, 9, -1}, {10, 4, 9, 6, 4, 10, 11, 2, 3, -1},
	{4, 8, 3, 4, 3, 10, 3, 1, 10, 6, 4, 10, -1}, {1, 10, 0, 10, 6, 0, 6, 4, 0, -1}, {4, 10, 6, 9, 10, 4, 0, 8, 3, -1},
	{4, 10, 6, 9, 10, 4, -1}, {6, 7, 11, 4, 5, 9, -1}, {4, 5, 9, 7, 11, 6, 3, 8, 0, -1},
	{1, 0, 5, 5, 0, 4, 11, 6, 7, -1}, {11, 6, 7, 5, 8, 4, 5, 3, 8, 5, 1, 3, -1}, {3, 2, 7, 7, 2, 6, 9, 4, 5, -1},
	{5, 9, 4, 0, 7, 8, 0, 6, 7, 0, 2, 6, -1}, {3, 2, 6, 3, 6, 7, 1, 0, 5, 0, 4, 5, -1}, {6, 1, 2, 5, 1, 6, 4, 7, 8, -1},
	{10, 2, 1, 6, 7, 11, 4, 5, 9, -1}, {0, 3, 8, 4, 5, 9, 11, 6, 7, 10, 2, 1, -1},
	{7, 11, 6, 2, 5, 10, 2, 4, 5, 2, 0, 4, -1}, {8, 4, 7, 5, 10, 6, 3, 11, 2, -1},
	{9, 4, 5, 7, 10, 6, 7, 1, 10, 7, 3, 1, -1}, {10, 6, 5, 7, 8, 4, 1, 9, 0, -1}, {4, 3, 0, 7, 3, 4, 6, 5, 10, -1},
	{10, 6, 5, 8, 4, 7, -1}, {9, 6, 5, 9, 11, 6, 9, 8, 11, -1}, {11, 6, 3, 3, 6, 0, 0, 6, 5, 0, 5, 9, -1},
	{11, 6, 5, 11, 5, 0, 5, 1, 0, 8, 11, 0, -1}, {11, 6, 3, 6, 5, 3, 5, 1, 3, -1},
	{9, 8, 5, 8, 3, 2, 5, 8, 2, 5, 2, 6, -1}, {5, 9, 6, 9, 0, 6, 0, 2, 6, -1}, {1, 6, 5, 2, 6, 1, 3, 0, 8, -1},
	{1, 6, 5, 2, 6, 1, -1}, {2, 1, 10, 9, 6, 5, 9, 11, 6, 9, 8, 11, -1}, {9, 0, 1, 3, 11, 2, 5, 10, 6, -1},
	{11, 0, 8, 2, 0, 11, 10, 6, 5, -1}, {3, 11, 2, 5, 10, 6, -1}, {1, 8, 3, 9, 8, 1, 5, 10, 6, -1},
	{6, 5, 10, 0, 1, 9, -1}, {8, 3, 0, 5, 10, 6, -1}, {6, 5, 10, -1}, {10, 5, 6, -1}, {0, 3, 8, 6, 10, 5, -1},
	{10, 5, 6, 9, 1, 0, -1}, {3, 8, 1, 1, 8, 9, 6, 10, 5, -1}, {2, 11, 3, 6, 10, 5, -1},
	{8, 0, 11, 11, 0, 2, 5, 6, 10, -1}, {1, 0, 9, 2, 11, 3, 6, 10, 5, -1}, {5, 6, 10, 11, 1, 2, 11, 9, 1, 11, 8, 9, -1},
	{5, 6, 1, 1, 6, 2, -1}, {5, 6, 1, 1, 6, 2, 8, 0, 3, -1}, {6, 9, 5, 6, 0, 9, 6, 2, 0, -1},
	{6, 2, 5, 2, 3, 8, 5, 2, 8, 5, 8, 9, -1}, {3, 6, 11, 3, 5, 6, 3, 1, 5, -1},
	{8, 0, 1, 8, 1, 6, 1, 5, 6, 11, 8, 6, -1}, {11, 3, 6, 6, 3, 5, 5, 3, 0, 5, 0, 9, -1},
	{5, 6, 9, 6, 11, 9, 11, 8, 9, -1}, {5, 6, 10, 7, 4, 8, -1}, {0, 3, 4, 4, 3, 7, 10, 5, 6, -1},
	{5, 6, 10, 4, 8, 7, 0, 9, 1, -1}, {6, 10, 5, 1, 4, 9, 1, 7, 4, 1, 3, 7, -1}, {7, 4, 8, 6, 10, 5, 2, 11, 3, -1},
	{10, 5, 6, 4, 11, 7, 4, 2, 11, 4, 0, 2, -1}, {4, 8, 7, 6, 10, 5, 3, 2, 11, 1, 0, 9, -1},
	{1, 2, 10, 11, 7, 6, 9, 5, 4, -1}, {2, 1, 6, 6, 1, 5, 8, 7, 4, -1}, {0, 3, 7, 0, 7, 4, 2, 1, 6, 1, 5, 6, -1},
	{8, 7, 4, 6, 9, 5, 6, 0, 9, 6, 2, 0, -1}, {7, 2, 3, 6, 2, 7, 5, 4, 9, -1},
	{4, 8, 7, 3, 6, 11, 3, 5, 6, 3, 1, 5, -1}, {5, 0, 1, 4, 0, 5, 7, 6, 11, -1}, {9, 5, 4, 6, 11, 7, 0, 8, 3, -1},
	{11, 7, 6, 9, 5, 4, -1}, {6, 10, 4, 4, 10, 9, -1}, {6, 10, 4, 4, 10, 9, 3, 8, 0, -1},
	{0, 10, 1, 0, 6, 10, 0, 4, 6, -1}, {6, 10, 1, 6, 1, 8, 1, 3, 8, 4, 6, 8, -1}, {9, 4, 10, 10, 4, 6, 3, 2, 11, -1},
	{2, 11, 8, 2, 8, 0, 6, 10, 4, 10, 9, 4, -1}, {11, 3, 2, 0, 10, 1, 0, 6, 10, 0, 4, 6, -1},
	{6, 8, 4, 11, 8, 6, 2, 10, 1, -1}, {4, 1, 9, 4, 2, 1, 4, 6, 2, -1}, {3, 8, 0, 4, 1, 9, 4, 2, 1, 4, 6, 2, -1},
	{6, 2, 4, 4, 2, 0, -1}, {3, 8, 2, 8, 4, 2, 4, 6, 2, -1}, {4, 6, 9, 6, 11, 3, 9, 6, 3, 9, 3, 1, -1},
	{8, 6, 11, 4, 6, 8, 9, 0, 1, -1}, {11, 3, 6, 3, 0, 6, 0, 4, 6, -1}, {8, 6, 11, 4, 6, 8, -1},
	{10, 7, 6, 10, 8, 7, 10, 9, 8, -1}, {3, 7, 0, 7, 6, 10, 0, 7, 10, 0, 10, 9, -1},
	{6, 10, 7, 7, 10, 8, 8, 10, 1, 8, 1, 0, -1}, {6, 10, 7, 10, 1, 7, 1, 3, 7, -1},
	{3, 2, 11, 10, 7, 6, 10, 8, 7, 10, 9, 8, -1}, {2, 9, 0, 10, 9, 2, 6, 11, 7, -1}, {0, 8, 3, 7, 6, 11, 1, 2, 10, -1},
	{7, 6, 11, 1, 2, 10, -1}, {2, 1, 9, 2, 9, 7, 9, 8, 7, 6, 2, 7, -1}, {2, 7, 6, 3, 7, 2, 0, 1, 9, -1},
	{8, 7, 0, 7, 6, 0, 6, 2, 0, -1}, {7, 2, 3, 6, 2, 7, -1}, {8, 1, 9, 3, 1, 8, 11, 7, 6, -1}, {11, 7, 6, 1, 9, 0, -1},
	{6, 11, 7, 0, 8, 3, -1}, {11, 7, 6, -1}, {7, 11, 5, 5, 11, 10, -1}, {10, 5, 11, 11, 5, 7, 0, 3, 8, -1},
	{7, 11, 5, 5, 11, 10, 0, 9, 1, -1}, {7, 11, 10, 7, 10, 5, 3, 8, 1, 8, 9, 1, -1}, {5, 2, 10, 5, 3, 2, 5, 7, 3, -1},
	{5, 7, 10, 7, 8, 0, 10, 7, 0, 10, 0, 2, -1}, {0, 9, 1, 5, 2, 10, 5, 3, 2, 5, 7, 3, -1},
	{9, 7, 8, 5, 7, 9, 10, 1, 2, -1}, {1, 11, 2, 1, 7, 11, 1, 5, 7, -1}, {8, 0, 3, 1, 11, 2, 1, 7, 11, 1, 5, 7, -1},
	{7, 11, 2, 7, 2, 9, 2, 0, 9, 5, 7, 9, -1}, {7, 9, 5, 8, 9, 7, 3, 11, 2, -1}, {3, 1, 7, 7, 1, 5, -1},
	{8, 0, 7, 0, 1, 7, 1, 5, 7, -1}, {0, 9, 3, 9, 5, 3, 5, 7, 3, -1}, {9, 7, 8, 5, 7, 9, -1},
	{8, 5, 4, 8, 10, 5, 8, 11, 10, -1}, {0, 3, 11, 0, 11, 5, 11, 10, 5, 4, 0, 5, -1},
	{1, 0, 9, 8, 5, 4, 8, 10, 5, 8, 11, 10, -1}, {10, 3, 11, 1, 3, 10, 9, 5, 4, -1},
	{3, 2, 8, 8, 2, 4, 4, 2, 10, 4, 10, 5, -1}, {10, 5, 2, 5, 4, 2, 4, 0, 2, -1}, {5, 4, 9, 8, 3, 0, 10, 1, 2, -1},
	{2, 10, 1, 4, 9, 5, -1}, {8, 11, 4, 11, 2, 1, 4, 11, 1, 4, 1, 5, -1}, {0, 5, 4, 1, 5, 0, 2, 3, 11, -1},
	{0, 11, 2, 8, 11, 0, 4, 9, 5, -1}, {5, 4, 9, 2, 3, 11, -1}, {4, 8, 5, 8, 3, 5, 3, 1, 5, -1}, {0, 5, 4, 1, 5, 0, -1},
	{5, 4, 9, 3, 0, 8, -1}, {5, 4, 9, -1}, {11, 4, 7, 11, 9, 4, 11, 10, 9, -1},
	{0, 3, 8, 11, 4, 7, 11, 9, 4, 11, 10, 9, -1}, {11, 10, 7, 10, 1, 0, 7, 10, 0, 7, 0, 4, -1},
	{3, 10, 1, 11, 10, 3, 7, 8, 4, -1}, {3, 2, 10, 3, 10, 4, 10, 9, 4, 7, 3, 4, -1}, {9, 2, 10, 0, 2, 9, 8, 4, 7, -1},
	{3, 4, 7, 0, 4, 3, 1, 2, 10, -1}, {7, 8, 4, 10, 1, 2, -1}, {7, 11, 4, 4, 11, 9, 9, 11, 2, 9, 2, 1, -1},
	{1, 9, 0, 4, 7, 8, 2, 3, 11, -1}, {7, 11, 4, 11, 2, 4, 2, 0, 4, -1}, {4, 7, 8, 2, 3, 11, -1},
	{9, 4, 1, 4, 7, 1, 7, 3, 1, -1}, {7, 8, 4, 1, 9, 0, -1}, {3, 4, 7, 0, 4, 3, -1}, {7, 8, 4, -1},
	{11, 10, 8, 8, 10, 9, -1}, {0, 3, 9, 3, 11, 9, 11, 10, 9, -1}, {1, 0, 10, 0, 8, 10, 8, 11, 10, -1},
	{10, 3, 11, 1, 3, 10, -1}, {3, 2, 8, 2, 10, 8, 10, 9, 8, -1}, {9, 2, 10, 0, 2, 9, -1}, {8, 3, 0, 10, 1, 2, -1},
	{2, 10, 1, -1}, {2, 1, 11, 1, 9, 11, 9, 8, 11, -1}, {11, 2, 3, 9, 0, 1, -1}, {11, 0, 8, 2, 0, 11, -1},
	{3, 11, 2, -1}, {1, 8, 3, 9, 8, 1, -1}, {1, 9, 0, -1}, {8, 3, 0, -1}, {-1}};

MarchingCubes::MarchingCubes(int displayWidth, int displayHeight, Scene* scene, ShaderProgram* shaderProg)
	:displayWidth(displayWidth),
	 displayHeight(displayHeight),
	 grid(gridSize, std::vector<std::vector<GridPoint>>(gridSize, std::vector<GridPoint>(gridSize))),
	 scene(scene),
	 shaderProg(shaderProg)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	// Attribute 0: vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	// Attribute 1: vertex normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	// Attribute 2: vertex color
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MarchingCubes::drawUI(bool& dirty)
{
	if (ImGui::InputInt("grid size", &gridSize))
	{
		dirty = true;
		grid = std::vector<std::vector<std::vector<GridPoint>>>(gridSize, std::vector<std::vector<GridPoint>>(gridSize, std::vector<GridPoint>(gridSize)));
	}
	if (ImGui::InputFloat("grid spacing", &gridSpacing)) dirty = true;
}

void MarchingCubes::render() const
{
	//sidenote: we mirror the x-axis here to align the coorinate system used by glm (right-handed)
	//with that of the raymarchers (left-handed)

	shaderProg->use();

	glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(-1.0f, 1.0f, 1.0f));

	glm::vec3 camPos = scene->getCamPos();
	camPos.x *= -1;

	glm::vec3 lookAt = scene->getLookAt();
	lookAt.x *= -1;

	glm::mat4 view = glm::lookAt(camPos, lookAt, glm::vec3(0, 1, 0));
	glm::mat4 proj = glm::perspective(glm::radians(90.0f), (float)displayWidth / (float)displayHeight, 0.1f, 100.0f);

	glm::vec3 lightPos = scene->getLightPos();
	lightPos.x *= -1;

	glm::vec3 lightColor = scene->getSpecColor();

	//pass uniforms
	shaderProg->passUniform4x4floatMatrix("model", model);
	shaderProg->passUniform4x4floatMatrix("view", view);
	shaderProg->passUniform4x4floatMatrix("projection", proj);
	shaderProg->passUniform3floatVector("lightPos", lightPos);
	shaderProg->passUniform3floatVector("viewPos", camPos);
	shaderProg->passUniform3floatVector("lightColor", lightColor);

	//draw buffers
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void MarchingCubes::regenerateMarchingCubes()
{
	for (int x = 0; x < gridSize; x++)
	{
		for (int y = 0; y < gridSize; y++)
		{
			for (int z = 0; z < gridSize; z++)
			{
				float posX = (x - gridSize / 2.0f) * gridSpacing;
				float posY = (y - gridSize / 2.0f) * gridSpacing;
				float posZ = (z - gridSize / 2.0f) * gridSpacing;

				glm::vec3 worldPoint(posX, posY, posZ);
				auto mapResult = scene->map(worldPoint);
				float dist = mapResult.first;
				glm::vec3 col = mapResult.second;
				grid[x][y][z] = { glm::vec3(posX, posY, posZ), dist, col };
			}
		}
	}

	marchingCubes();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
}

void MarchingCubes::marchingCubes()
{
	vertices.clear();
	vertices.reserve(100000); //TODO: use actual nr of points used

	for (int x = 0; x < gridSize - 1; x++)
	{
		for (int y = 0; y < gridSize - 1; y++)
		{
			for (int z = 0; z < gridSize - 1; z++)
			{
				GridPoint cubeCorners[8] = {
					grid[x][y][z],
					grid[x+1][y][z],
					grid[x][y+1][z],
					grid[x+1][y+1][z],
					grid[x][y][z+1],
					grid[x+1][y][z+1],
					grid[x][y+1][z+1],
					grid[x+1][y+1][z+1]};

				//determine which corners of the cube are inside of a shape
				int cubeIndex = 0; //bitflags: one bit for each corner
				for (int i = 0; i < 8; i++)
				{
					if (cubeCorners[i].value < 0.0f)
						cubeIndex |= (1 << i);
				}

				//cube is entirely outside of shape
				if (edgeTable[cubeIndex] == 0)
					continue;

				//?
				Vertex edgeVertices[12];
				for (int i = 0; i < 12; i++)
				{
					if (edgeTable[cubeIndex] & (1 << i))
					{
						int v1 = edgeVertexIndices[i][0];
						int v2 = edgeVertexIndices[i][1];
						Vertex v = interpolateVertex(cubeCorners[v1], cubeCorners[v2]);
						v.normal = getNormal(v.position);
						edgeVertices[i] = v;
					}
				}

				//triangulate
				int idx = 0;
				while (triTable[cubeIndex][idx] != -1)
				{
					int a = triTable[cubeIndex][idx];
					int b = triTable[cubeIndex][idx + 1];
					int c = triTable[cubeIndex][idx + 2];

					vertices.push_back(edgeVertices[a]);
					vertices.push_back(edgeVertices[b]);
					vertices.push_back(edgeVertices[c]);

					idx += 3;
				}
			}
		}
	}
}

glm::vec3 MarchingCubes::interpolate(GridPoint p1, GridPoint p2) const
{
	float v1 = p1.value;
	float v2 = p2.value;

	float t = (0.0f - v1) / (v2 - v1);
	return p1.position + t * (p2.position - p1.position);
}

MarchingCubes::Vertex MarchingCubes::interpolateVertex(const GridPoint& p1,
													   const GridPoint& p2)
{
	float t = (0.0f - p1.value) / (p2.value - p1.value);
	Vertex v{};
	v.position = p1.position + t * (p2.position - p1.position);
	v.color = p1.color + t * (p2.color - p1.color);
	return v;
}

glm::vec3 MarchingCubes::getNormal(glm::vec3 point) const
{
	glm::vec2 e(0.001, 0.0);

	float comp1 = scene->map(point - glm::vec3(e.x, e.y, e.y)).first;
	float comp2 = scene->map(point - glm::vec3(e.y, e.x, e.y)).first;
	float comp3 = scene->map(point - glm::vec3(e.y, e.y, e.x)).first;
	glm::vec3 n = glm::vec3(scene->map(point).first) - glm::vec3(comp1, comp2, comp3);
	return glm::normalize(n);
}