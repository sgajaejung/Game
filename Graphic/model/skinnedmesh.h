#pragma once


namespace graphic
{

	class cSkinnedMesh : public cMesh
	{
	public:
		cSkinnedMesh(const int id, const vector<Matrix44> &palette, const sRawMesh &raw);
		virtual ~cSkinnedMesh();

		virtual void Render(const Matrix44 &parentTm) override;
		virtual void RenderShader( cShader &shader, const Matrix44 &parentTm ) override;


	protected:
		void ApplyPalette();
		void ApplyPaletteShader(cShader &shader);


	private:
		const sRawMesh &m_rawMesh;
		const vector<Matrix44> &m_palette;
	};

}
