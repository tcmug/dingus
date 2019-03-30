

ui_shader = SHADER({vert = 'ui.vert', frag = 'ui.frag'})
flat_shader = SHADER({vert = 'flat.vert', frag = 'flat.frag'})
edge_shader = SHADER({vert = 'edges.vert', frag = 'edges.frag'})

monkey = MODEL({file = "monkey.obj"})
ship = MODEL({file = "ship.obj"})

bla = function (component)
    print("clicked")
    insert(parent(component),  TEXT({ x = 5, y = 40, w = 100, h = 20, text = "Hi!" }) )
end

screen = ENTITY({
    w = 800,
    h = 600,
    children = {
        VIEW({
            w = 800,
            h = 600,
            depth=1,
            children={
                SCENE({
                    objects={
                        OBJECT({
                            z=-5,
                            model=monkey,
                            shader=flat_shader,
                            children={
                                OBJECT({
                                    model=monkey,
                                    shader=edge_shader
                                })                      
                            }
                        }),
                        OBJECT({
                            x=-5,
                            model=ship,
                            shader=flat_shader
                        })                        
                    }
                })
            }
        }),     
        VIEW({
            w = 800,
            h = 600,
            children = {
                TEXT({ x = 5, y = 5, w = 100, h = 20, text = "Quit", click = function () quit() end }),
                TEXT({ x = 5, y = 105, w = 100, h = 20, text = "Test", click = bla }),
                TEXT({ x = 5, y = 210, w = 100, h = 20, text = "Test 2", click = bla }) 
            }
        })
    }
})


