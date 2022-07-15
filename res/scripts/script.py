from alien3d import * 

s = Sprite("character", "../res/sprite.png", True)
t = s.getComponent("transform")
t.setPosition(0.0,10.0)
v = Vec3(0.2,0.2,0.2)
t.setScale(v)
b = PhysicsBody(s, BodyType.DYNAMIC)
s.attachComponent(b,"physics_body")

ground = Sprite("ground", "../res/ground.png", True)
gt = ground.getComponent("transform")
gt.setPosition(0.0,0.0)
v2 = Vec3(10.0,0.04,0.1)
gt.setScale(v2)
gb = PhysicsBody(ground, BodyType.STATIC)
ground.attachComponent(gb, "physics_body")
