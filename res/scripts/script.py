from alien3d import * 

s = Sprite("test", "../res/sprite.png", True)
t = s.getComponent("transform")
t.setPosition(0.0,10.0)
b = PhysicsBody(s, BodyType.DYNAMIC)
s.attachComponent(b,"physics_body")