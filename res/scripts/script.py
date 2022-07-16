from alien3d import * 

class GameScript:
    def __init__(self):
        
        pass

    def start(self):
        self.exp = Sprite("explosion", "../res/assets/explosion_3.png", True)
        t = self.exp.getComponent("transform")
        t.setPosition(-2.0,0.0)
        self.anim = SpriteAnimation(self.exp,682,682)
        self.exp.attachComponent(self.anim,"sprite_animation")

        self.s = Sprite("character", "../res/sprite.png", True)
        t = self.s.getComponent("transform")
        t.setPosition(0.0,10.0)
        v = Vec3(0.5,0.5,0.5)
        t.setScale(v)
        self.b = PhysicsBody(self.s, BodyType.DYNAMIC)
        self.s.attachComponent(self.b,"physics_body")

        self.ground = Sprite("ground", "../res/ground.png", True)
        gt = self.ground.getComponent("transform")
        gt.setPosition(0.0,0.0)
        # v2 = Vec3(10.0,0.04,0.1)
        # gt.setScale(v2)
        self.gb = PhysicsBody(self.ground, BodyType.STATIC)
        self.ground.attachComponent(self.gb, "physics_body")
        pass

    def update(self):
        print("Update call")
        pass



