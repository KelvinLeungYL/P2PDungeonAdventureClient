#include "Sprite.h"
#include "Misc.h"
#include "GpuResourceUtils.h"
#include "tentacle_lib.h"

// �R���X�g���N�^
Sprite::Sprite(ID3D11Device* device)
	:Sprite(device, nullptr)
{
}

// �R���X�g���N�^
Sprite::Sprite(ID3D11Device* device, const char* filename)
{
	HRESULT hr = S_OK;

	// ���_�o�b�t�@�̐���
	{
		// ���_�o�b�t�@���쐬���邽�߂̐ݒ�I�v�V����
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.ByteWidth = sizeof(Vertex) * 4; // 4���_
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		// ���_�o�b�t�@�I�u�I�u�W�F�N�g�̐���
		hr = device->CreateBuffer(&buffer_desc, nullptr, vertexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ���_�V�F�[�_�[
	{
		// ���̓��C�A�E�g
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		hr = GpuResourceUtils::LoadVertexShader(
			device,
			"Data/Shader/SpriteVS.cso",
			inputElementDesc,
			ARRAYSIZE(inputElementDesc),
			inputLayout.GetAddressOf(),
			vertexShader.GetAddressOf()
		);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �s�N�Z���V�F�[�_�[
	{
		hr = GpuResourceUtils::LoadPixelShader(
			device,
			"Data/Shader/SpritePS.cso",
			pixelShader.GetAddressOf()
		);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �e�N�X�`������
	if (filename != nullptr)
	{
		// �e�N�X�`���t�@�C���ǂݍ���
		D3D11_TEXTURE2D_DESC desc;
		hr = GpuResourceUtils::LoadTexture(
			device,
			filename,
			shaderResourceView.GetAddressOf(),
			&desc
		);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		textureSize = {
			static_cast<float>(desc.Width),
			static_cast<float>(desc.Height)
		};
	}
	else
	{
		// �_�~�[�e�N�X�`������
		D3D11_TEXTURE2D_DESC desc;
		hr = GpuResourceUtils::CreateDummyTexture(device, 0xFFFFFFFF, shaderResourceView.GetAddressOf(), &desc);

		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		textureSize = {
			static_cast<float>(desc.Width),
			static_cast<float>(desc.Height)
		};
	}
}

void Sprite::Update(
	float dx, float dy,
	float dw, float dh,
	float sx, float sy,
	float sw, float sh,
	float angle,
	float r, float g, float b, float a) const
{
	ID3D11DeviceContext* immediate_context = Graphics::Instance().GetDeviceContext();
	{
		// �X�v���C�g���\������S���_�̃X�N���[�����W���v�Z����
		DirectX::XMFLOAT2 positions[] = {
			DirectX::XMFLOAT2(dx,      dy),			// ����
			DirectX::XMFLOAT2(dx + dw, dy),			// �E��
			DirectX::XMFLOAT2(dx,      dy + dh),	// ����
			DirectX::XMFLOAT2(dx + dw, dy + dh),	// �E��
		};

		// �X�v���C�g���\������S���_�̃e�N�X�`�����W���v�Z����
		DirectX::XMFLOAT2 texcoords[] = {
			DirectX::XMFLOAT2(sx,      sy),			// ����
			DirectX::XMFLOAT2(sx + sw, sy),			// �E��
			DirectX::XMFLOAT2(sx,      sy + sh),	// ����
			DirectX::XMFLOAT2(sx + sw, sy + sh),	// �E��
		};

		// �X�v���C�g�̒��S�ŉ�]�����邽�߂ɂS���_�̒��S�ʒu��
		// ���_(0, 0)�ɂȂ�悤�Ɉ�U���_���ړ�������B
		float mx = dx + dw * 0.5f;
		float my = dy + dh * 0.5f;
		for (auto& p : positions)
		{
			p.x -= mx;
			p.y -= my;
		}

		// ���_����]������
		float theta = angle;	// �p�x�����W�A��(��)�ɕϊ�
		//float theta = DirectX::XMConvertToRadians(angle);	// �p�x�����W�A��(��)�ɕϊ�
		float c = cosf(theta);
		float s = sinf(theta);
		for (auto& p : positions)
		{
			DirectX::XMFLOAT2 r = p;
			p.x = c * r.x + -s * r.y;
			p.y = s * r.x + c * r.y;
		}

		// ��]�̂��߂Ɉړ����������_�����̈ʒu�ɖ߂�
		for (auto& p : positions)
		{
			p.x += mx;
			p.y += my;
		}

		// ���ݐݒ肳��Ă���r���[�|�[�g����X�N���[���T�C�Y���擾����B
		D3D11_VIEWPORT viewport;
		UINT numViewports = 1;
		immediate_context->RSGetViewports(&numViewports, &viewport);
		float screen_width = viewport.Width;
		float screen_height = viewport.Height;

		// �X�N���[�����W�n����NDC���W�n�֕ϊ�����B
		for (auto& p : positions)
		{
			p.x = 2.0f * p.x / screen_width - 1.0f;
			p.y = 1.0f - 2.0f * p.y / screen_height;
		}

		// ���_�o�b�t�@�̓��e�̕ҏW���J�n����B
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		HRESULT hr = immediate_context->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// pData��ҏW���邱�ƂŒ��_�f�[�^�̓��e�����������邱�Ƃ��ł���B
		Vertex* v = static_cast<Vertex*>(mappedBuffer.pData);
		for (int i = 0; i < 4; ++i)
		{
			v[i].position.x = positions[i].x;
			v[i].position.y = positions[i].y;
			v[i].position.z = 0.0f;

			v[i].color.x = r;
			v[i].color.y = g;
			v[i].color.z = b;
			v[i].color.w = a;

			v[i].texcoord.x = texcoords[i].x / textureSize.x;
			v[i].texcoord.y = texcoords[i].y / textureSize.y;
		}

		// ���_�o�b�t�@�̓��e�̕ҏW���I������B
		immediate_context->Unmap(vertexBuffer.Get(), 0);
	}
}

void Sprite::Render(
	ID3D11DeviceContext* dc,
	float dx, float dy,					// �ʒu
	float dz,							// ���s
	float dw, float dh,					// �T�C�Y
	float sx, float sy,					// �摜�؂蔲���ʒu
	float sw, float sh,					// �摜�؂蔲���T�C�Y
	float angle,						// �p�x
	float r, float g, float b, float a	// �F
) const
{
	// ���_���W
	DirectX::XMFLOAT2 positions[] = {
		DirectX::XMFLOAT2(dx, dy),				// ����
		DirectX::XMFLOAT2(dx + dw, dy),			// �E��
		DirectX::XMFLOAT2(dx, dy + dh),			// ����
		DirectX::XMFLOAT2(dx + dw, dy + dh),	// �E��
	};

	// �e�N�X�`�����W
	DirectX::XMFLOAT2 texcorrds[] = {
		DirectX::XMFLOAT2(sx, sy),	// ����
		DirectX::XMFLOAT2(sx + sw, sy),	// �E��
		DirectX::XMFLOAT2(sx, sy + sh),	// ����
		DirectX::XMFLOAT2(sx + sw, sy + sh),	// �E��
	};

	// �X�v���C�g�̒��S�ŉ�]������ׂ�4���_�̒��S�ʒu�����_�ɂȂ�悤�ɂ������񒸓_���ړ�������
	float mx = dx + dw * 0.5f;
	float my = dy + dh * 0.5f;
	for (auto& p : positions)
	{
		p.x -= mx;
		p.y -= my;
	}

	// ���_����]������
	float theta = angle;
	//float theta = DirectX::XMConvertToRadians(angle);
	float c = cosf(theta);
	float s = sinf(theta);
	for (auto& p : positions)
	{
		DirectX::XMFLOAT2 r = p;
		p.x = c * r.x - s * r.y;
		p.y = s * r.x + c * r.y;
	}

	// ���̈ʒu�ɖ߂�
	for (auto& p : positions)
	{
		p.x += mx;
		p.y += my;
	}

	// ���ݐݒ肳��Ă���r���[�|�[�g����X�N���[���T�C�Y���擾
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);
	float screenWidth = viewport.Width;
	float screenHeight = viewport.Height;

	// �X�N���[�����W����NDC���W�n�֕ϊ�
	for (DirectX::XMFLOAT2& p : positions)
	{
		p.x = 2.0f * p.x / screenWidth - 1.0f;
		p.y = 1.0f - 2.0f * p.y / screenHeight;
	}

	// ���_�o�b�t�@�̓��e�̕ҏW���J�n����
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	HRESULT hr = dc->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// ���_�o�b�t�@�̓��e��ҏW
	Vertex* v = static_cast<Vertex*>(mappedSubresource.pData);
	for (int i = 0; i < 4; ++i)
	{
		v[i].position.x = positions[i].x;
		v[i].position.y = positions[i].y;
		v[i].position.z = dz;					// ���s

		v[i].color.x = r;
		v[i].color.y = g;
		v[i].color.z = b;
		v[i].color.w = a;

		v[i].texcoord.x = texcorrds[i].x / textureSize.x;
		v[i].texcoord.y = texcorrds[i].y / textureSize.y;
	}

	// ���_�o�b�t�@�̓��e�̕ҏW���I������
	dc->Unmap(vertexBuffer.Get(), 0);

	// GPU�ɕ`�悷��ׂ̃f�[�^��n��
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	dc->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	dc->IASetInputLayout(inputLayout.Get());
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	dc->VSSetShader(vertexShader.Get(), nullptr, 0);
	dc->PSSetShader(pixelShader.Get(), nullptr, 0);
	dc->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());

	// �����_�[�X�e�[�g�ݒ�
	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	dc->OMSetBlendState(T_GRAPHICS.GetRenderState()->GetBlendState(BlendState::Transparency), blend_factor, 0xFFFFFFFF);
	dc->OMSetDepthStencilState(T_GRAPHICS.GetRenderState()->GetDepthStencilState(DepthState::TestAndWrite), 0);
	dc->RSSetState(T_GRAPHICS.GetRenderState()->GetRasterizerState(RasterizerState::SolidCullBack));

	// �`��
	dc->Draw(4, 0);

	dc->VSSetShader(nullptr, nullptr, 0);
	dc->PSSetShader(nullptr, nullptr, 0);
	dc->IASetInputLayout(nullptr);
}

void Sprite::Render(
	ID3D11DeviceContext* dc,
	float dx, float dy,					// �ʒu
	float dz,							// ���s
	float dw, float dh,					// �T�C�Y
	float angle,						// �p�x
	float r, float g, float b, float a	// �F
) const
{
	Render(dc, dx, dy, dz, dw, dh, 0, 0, textureSize.x, textureSize.y, angle, r, g, b, a);
}

void Sprite::Render(
	ID3D11DeviceContext* dc,
	float dx, float dy,					// �ʒu
	float dz,							// ���s
	float dw, float dh					// �T�C�Y
) const
{
	Render(dc, dx, dy, dz, dw, dh, 0, 0, textureSize.x, textureSize.y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

// �V�F�[�_�[���\�[�X�r���[�̐ݒ�
void Sprite::SetShaderResourceView(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& srv, int texWidth, int texHeight)
{
	shaderResourceView = srv;
	textureSize = {
		static_cast<float>(texWidth),
		static_cast<float>(texHeight)
	};
}