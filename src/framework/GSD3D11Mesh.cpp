﻿#include "mgf.h"
#ifdef MGF_GS_D3D11

#include "Log.h"
#include "GSD3D11.h"
#include "GSD3D11Mesh.h"

using namespace mgf;

GSD3D11Mesh::GSD3D11Mesh(GSMeshInfo* mi, ID3D11Device* d, ID3D11DeviceContext* dc)
{
	m_d3d11DevCon = dc;
	m_meshInfo = *mi;
	Mesh* mesh = (Mesh*)m_meshInfo.m_meshPtr;
	m_vertexType = mesh->m_vertexType;

	D3D11_BUFFER_DESC	vbd, ibd;

	ZeroMemory(&vbd, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&ibd, sizeof(D3D11_BUFFER_DESC));

	vbd.Usage = D3D11_USAGE_DEFAULT;
	//vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;

	ibd.Usage = D3D11_USAGE_DEFAULT;
	//ibd.Usage = D3D11_USAGE_DYNAMIC;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	//vbd.CPUAccessFlags = 0;
	//ibd.CPUAccessFlags = 0; //D3D11_CPU_ACCESS_WRITE

	D3D11_SUBRESOURCE_DATA	vData, iData;
	ZeroMemory(&vData, sizeof(D3D11_SUBRESOURCE_DATA));
	ZeroMemory(&iData, sizeof(D3D11_SUBRESOURCE_DATA));
	HRESULT	hr = 0;


	vbd.ByteWidth = mesh->m_stride * mesh->m_vCount;
	vData.pSysMem = &mesh->m_vertices[0];

	hr = d->CreateBuffer(&vbd, &vData, &m_vBuffer);
	if (FAILED(hr))
		LogWriteError("Can't create Direct3D 11 vertex buffer\n");

	m_stride = mesh->m_stride;
	m_iCount = mesh->m_iCount;
	if (mesh->m_indices)
	{
		uint32_t index_sizeof = sizeof(uint16_t);
		m_indexType = DXGI_FORMAT_R16_UINT;
		if (mesh->m_indexType == MeshIndexType::_u32)
		{
			m_indexType = DXGI_FORMAT_R32_UINT;
			index_sizeof = sizeof(uint32_t);
		}
		ibd.ByteWidth = index_sizeof * mesh->m_iCount;
		iData.pSysMem = &mesh->m_indices[0];


		hr = d->CreateBuffer(&ibd, &iData, &m_iBuffer);
		if (FAILED(hr))
			LogWriteError("Can't create Direct3D 11 index buffer\n");
	}
}

GSD3D11Mesh::~GSD3D11Mesh()
{
	if (m_vBuffer)
	{
		m_vBuffer->Release();
		m_vBuffer = 0;
	}

	if (m_iBuffer)
	{
		m_iBuffer->Release();
		m_iBuffer = 0;
	}
}


void GSD3D11Mesh::MapModelForWriteVerts(uint8_t** v_ptr)
{
	static D3D11_MAPPED_SUBRESOURCE mapData;
	auto hr = m_d3d11DevCon->Map(
		m_vBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mapData
	);
	if (FAILED(hr))
	{
		printf("Can not lock D3D11 render model buffer. Code : %u\n", hr);
		return;
	}
	*v_ptr = (uint8_t*)mapData.pData;
	m_lockedResource = m_vBuffer;
}

void GSD3D11Mesh::UnmapModelForWriteVerts()
{
	m_d3d11DevCon->Unmap(m_lockedResource, 0);
	m_lockedResource = nullptr;
}

#endif

