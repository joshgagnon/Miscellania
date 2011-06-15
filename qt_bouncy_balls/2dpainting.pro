QT += opengl
HEADERS = glwidget.h \
    window.h \
    ball.h \
    vector.h \
    game.h \
    usp.h
SOURCES = glwidget.cpp \
    main.cpp \
    window.cpp \
    ball.cpp \
    vector.cpp \
    game.cpp \
    usp.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/2dpainting
sources.files = $$SOURCES \
    $$HEADERS \
    $$RESOURCES \
    $$FORMS \
    2dpainting.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/opengl/2dpainting
INSTALLS += target \
    sources
