/*
* Copyright (c) 2016-2016 Irlan Robson http://www.irlan.net
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef GYRO_TEST_H
#define GYRO_TEST_H

#include <testbed/tests/quickhull_test.h>

extern DebugDraw* g_debugDraw;
extern Camera g_camera;
extern Settings g_settings;

class GyroTest : public Test
{
public:
	GyroTest()
	{
		{
			b3StackArray<b3Vec3, 32> points;
			ConstructCylinder(points, 0.95f, 4.0f);

			const u32 size = qhGetMemorySize(points.Count());
			void* p = b3Alloc(size);
			
			qhHull hull;
			hull.Construct(p, points);
			m_cylinderHull = ConvertHull(hull);
			
			b3Free(p);
		}

		{
			b3BodyDef bd;
			b3Body* ground = m_world.CreateBody(bd);

			b3HullShape hs;
			hs.m_hull = &m_groundHull;

			b3ShapeDef sd;
			sd.shape = &hs;

			ground->CreateShape(sd);
		}

		{
			b3BodyDef bdef;
			bdef.type = e_dynamicBody;
			bdef.orientation.Set(b3Vec3(1.0f, 0.0f, 0.0f), 0.5f * B3_PI);
			bdef.position.Set(0.0f, 10.0f, 0.0f);
			bdef.angularVelocity.Set(0.0f, 0.0f, 4.0f * B3_PI);

			b3Body* body = m_world.CreateBody(bdef);

			{
				b3Transform xf;
				xf.position.SetZero();
				xf.rotation = b3Diagonal(1.0f, 0.5f, 7.0f);
				m_rotorBox.SetTransform(xf);

				b3HullShape hull;
				hull.m_hull = &m_rotorBox;

				b3ShapeDef sdef;
				sdef.density = 0.1f;
				sdef.shape = &hull;

				body->CreateShape(sdef);
			}

			{
				b3HullShape hull;
				hull.m_hull = &m_cylinderHull;

				b3ShapeDef sdef;
				sdef.density = 0.2f;
				sdef.shape = &hull;

				body->CreateShape(sdef);
			}
		}

		m_world.SetGravity(b3Vec3(0.0f, 0.0f, 0.0f));
	}

	~GyroTest()
	{
		{
			b3Free(m_cylinderHull.vertices);
			b3Free(m_cylinderHull.edges);
			b3Free(m_cylinderHull.faces);
			b3Free(m_cylinderHull.planes);
		}
	}

	static Test* Create()
	{
		return new GyroTest();
	}

	b3BoxHull m_rotorBox;
	b3Hull m_cylinderHull;
};

#endif