#pragma once
#include "Sphere.h"

class Disc : public Sphere
{
  const GLint radiusLoc;
  const GLint centerLoc;
  const GLint scaleLoc;
  const GLint offsetLoc;
  const GLint deltaLoc;

public:
  Disc(int slices, int stacks, const char *vert = "disc.vert", const char *frag = "discSAD.frag", const char *geom = nullptr);
  virtual ~Disc();
  virtual void draw(const GgMatrix &mp, const GgMatrix &mv) const;
};
