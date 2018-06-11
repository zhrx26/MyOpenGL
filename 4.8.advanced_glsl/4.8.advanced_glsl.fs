#version 330 core

out vec4 FragColor;

void main()
{
//    FragColor = vec4(1.0, 0.0, 0.0, 1.0);

//    // gl_FragCoord的x和y分量是片段的窗口空间坐标
//    if(gl_FragCoord.x < 400)
//        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
//    else
//        FragColor = vec4(0.0, 1.0, 0.0, 1.0);    

    // 根据顶点的环绕顺序来判断当前片段是正向还是背向面
    if (gl_FrontFacing)
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    else
        FragColor = vec4(0.0, 1.0, 0.0, 1.0);   
}
