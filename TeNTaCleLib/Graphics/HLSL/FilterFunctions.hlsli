//--------------------------------------------
//	RGB�F��Ԃ̐��l����HSV�F��Ԃ̐��l�ւ̕ϊ��֐�
//--------------------------------------------
//rgb:RGB�F��Ԃ̐��l
float3 RGB2HSV(float3 rgb)
{
    float3 hsv = 0;
	// V�i���x�j �̍ő�l�ƍŏ��l�����߂܂��傤�B
    float Vmax = max(max(rgb.r, rgb.g), rgb.b);

    float Vmin = min(min(rgb.r, rgb.g), rgb.b);

	// �ő�l�ƍŏ��l�̍�
    float delta = Vmax - Vmin;

	// V�i���x�j ��ԋ����F��V�l�ɂ���
    hsv.z = Vmax;

	// S�i�ʓx�j �ő�l�ƍŏ��l�̍��𐳋K�����ċ��߂�
    if (Vmax != 0.0)
    {
        hsv.y = (delta / Vmax) * step(0, Vmax);
    }
    else
    {
        hsv.y = 0.0;
    }
    
    
	// H�i�F���j RGB�̂����ő�l�ƍŏ��l�̍����狁�߂�
    if (hsv.y > 0.0f)
    {
		if (Vmax == rgb.r)
        {
            hsv.x = 60 * (rgb.g - rgb.b) / delta;

        }
        else  if (Vmax == rgb.g)
        {
            hsv.x = (60 * (rgb.b - rgb.r) / delta) + 120;
        }
        else if (Vmax == rgb.b)
        {
            hsv.x = (60 * (rgb.r - rgb.g) / delta) + 240;
        }

        while (hsv.x < 0)
        {
            hsv.x += 360;
        }
        while (hsv.x >= 360)
        {
            hsv.x -= 360;
        }
    }
    return hsv;
}


//--------------------------------------------
//	HSV�F��Ԃ̐��l����RGB�F��Ԃ̐��l�ւ̕ϊ��֐�
//--------------------------------------------
//hsv:HSV�F��Ԃ̐��l
float3 HSV2RGB(float3 hsv)
{
    float3 rgb = hsv.zzz;
    if (hsv.y == 0)
    {
		// S�i�ʓx�j��0�Ɠ������Ȃ�Ζ��F�������͊D�F
        return hsv.bbb;
    }
    else
    {
		// �F��H�i�F���j�̈ʒu��S�i�ʓx�j�AV�i���x�j����RGB�l���Z�o����
        float Vmax = hsv.z;
        float Vmin = Vmax - (hsv.y * Vmax);
        
        hsv.x %= 360; //	0�`359�ɕϊ�
        
        int Huei = floor(hsv.x) / 60;
        
        float Huef = hsv.x / 60 - Huei;
        
        float p = hsv.z * (1.0 - hsv.y);
        float q = hsv.z * (1.0 - hsv.y * Huef);
        float t = hsv.z * (1.0 - hsv.y * (1.0 - Huef));
        
        if (Huei == 0)
        {
            rgb.r = hsv.z;
            rgb.g = t;
            rgb.b = p;

        }
        else if (Huei == 1)
        {
            rgb.r = q;
            rgb.g = hsv.z;
            rgb.b = p;

        }
        else if (Huei == 2)
        {
            rgb.r = p;
            rgb.g = hsv.z;
            rgb.b = t;

        }
        else if (Huei == 3)
        {
            rgb.r = p;
            rgb.g = q;
            rgb.b = hsv.z;

        }
        else if (Huei == 4)
        {
            rgb.r = t;
            rgb.g = p;
            rgb.b = hsv.z;

        }
        else if (Huei == 5)
        {
            rgb.r = hsv.z;
            rgb.g = p;
            rgb.b = q;
        }
        else
        {
            // debug
            rgb = float3(1, 1, 1);
        }
    }
    return rgb;
}


//--------------------------------------------
//	RGB�F��Ԃ̐��l����P�x�l�ւ̕ϊ��֐�
//--------------------------------------------
//rgb:RGB�F��Ԃ̐��l
float RGB2Luminance(float3 rgb)
{
    static const float3 luminanceValue = float3(0.299f, 0.587f, 0.114f);
    return max(0.00001, dot(luminanceValue, rgb));
}
