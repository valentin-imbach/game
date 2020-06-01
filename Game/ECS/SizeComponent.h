//
//  SizeComponent.h
//  Game
//
//  Created by Valentin Imbach on 19.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

class SizeComponent : public Component {
public:
    pair<int> size;
    SizeComponent() {}
    SizeComponent(Entity* entity, pair<int> s = {1,1}) {
        size = s;
    }
};
