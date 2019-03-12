


bla = function (component)
    insert(parent(component),  TEXT({ x = 5, y = 40, w = 100, h = 20, text = "Hi!" }) )
end

screen =
    VIEW({ x = 0, y = 0,
        children = {
            TEXT({ x = 5, y = 5, w = 100, h = 20, text = "Quit", click = function () quit() end }),
            TEXT({ x = 5, y = 105, w = 100, h = 20, text = "Test", click = bla }) 
        }
    })
