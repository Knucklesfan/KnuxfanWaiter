#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 vUV;

// texture sampler
uniform sampler2D texture0;
uniform sampler2D texture1;

const vec3 _Colors = vec3(8.0);

float channelError(float col, float colMin, float colMax)
{
    float range = abs(colMin - colMax);
    float aRange = abs(col - colMin);
    return aRange /range;
}

float ditheredChannel(float error, vec2 ditherBlockUV)
{
    float pattern = texture(texture1, ditherBlockUV).r;
    if (error > pattern) {
        return 1.0;
    } else {
        return 0.0;
    }
}

vec4 mix(vec4 a, vec4 b, float amt)
{
    return ((1.0 - amt) * a) + (b * amt);
}

/// YUV/RGB color space calculations

vec3 RGBtoYUV(vec3 rgb) {
    vec3 yuv;
    yuv.r = rgb.r * 0.2126 + 0.7152 * rgb.g + 0.0722 * rgb.b;
    yuv.g = (rgb.b - yuv.r) / 1.8556;
    yuv.b = (rgb.r - yuv.r) / 1.5748;

    // Adjust to work on GPU
    yuv.gb += 0.5;

    return yuv;
}

vec3 YUVtoRGB(vec3 yuv) {
    yuv.gb -= 0.5;
    return vec3(
        yuv.r * 1.0 + yuv.g * 0.0 + yuv.b * 1.5748,
        yuv.r * 1.0 + yuv.g * -0.187324 + yuv.b * -0.468124,
        yuv.r * 1.0 + yuv.g * 1.8556 + yuv.b * 0.0);
}


vec3 ditherColor(vec3 col, vec2 uv, float xres, float yres) {
    vec3 yuv = RGBtoYUV(col);

    vec3 col1 = floor(yuv * _Colors) / _Colors;
    vec3 col2 = ceil(yuv * _Colors) / _Colors;
    
    // Calculate dither texture UV based on the input texture
    vec2 ditherBlockUV = uv * vec2(xres / 8.0, yres / 8.0);
   
    yuv.x = mix(col1.x, col2.x, ditheredChannel(channelError(yuv.x, col1.x, col2.x), ditherBlockUV));
    yuv.y = mix(col1.y, col2.y, ditheredChannel(channelError(yuv.y, col1.y, col2.y), ditherBlockUV));
    yuv.z = mix(col1.z, col2.z, ditheredChannel(channelError(yuv.z, col1.z, col2.z), ditherBlockUV));
    return(YUVtoRGB(yuv));
}

vec3 getPixel(vec2 uv) {
    return texture(texture0,uv).rgb;
}

void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = vUV;

    // Time varying pixel color
    vec3 col = getPixel(vUV);
    col = ditherColor(col, vUV, 848, 480);        

    // Output to screen
    FragColor = vec4(col,1.0);
}