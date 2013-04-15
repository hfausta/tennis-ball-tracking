#pragma once
#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>

namespace TennisTracking {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Problem
	/// </summary>
	public ref class Problem : public System::Windows::Forms::Form
	{
	public:
		Problem(std::string problems)
		{
			InitializeComponent();
			this->error->Text = msclr::interop::marshal_as<String^>(problems);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Problem()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  error;
	protected: 

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->error = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// error
			// 
			this->error->AutoSize = true;
			this->error->ForeColor = System::Drawing::Color::Red;
			this->error->Location = System::Drawing::Point(113, 102);
			this->error->Name = L"error";
			this->error->Size = System::Drawing::Size(32, 13);
			this->error->TabIndex = 0;
			this->error->Text = L"Error!";
			this->error->Click += gcnew System::EventHandler(this, &Problem::error_Click);
			// 
			// Problem
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 262);
			this->Controls->Add(this->error);
			this->Name = L"Problem";
			this->Text = L"Problem";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void error_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
	};
}
