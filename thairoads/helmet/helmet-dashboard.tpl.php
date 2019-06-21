<link href="https://fonts.googleapis.com/css?family=Prompt:300,400,500,600,700" rel="stylesheet">

<div class="main-helmet-dashboard">
  <div class="form-group -helmet filter">
    <label>ระบุพื้นที่</label>
    <select class="form-control area-filter">
    <?php
    foreach($province_list as $value) {
      print '<option value="'.$value->tid.'">';
      print $value->name;
      print '</option>';
    }
    ?>
    </select>
    <label>ปี พ.ศ.</label>

    <select class="form-control year-filter">

    </select>
    <input type="button" class="btn btn-primary" value="เลือก" id="dashboard_filter_btn">

    <div class="print-wrapper"><input type="button" value="<?php print t("print"); ?>" onclick="window.print();" class="print-btn -helmet"></div>
  </div><!-- /Form Group -->


  <div class="filter-info">
    <h3 class="filter-info-title">อัตราการสวมหมวกนิรภัย</h3><span class="area-info"></span> ปี พ.ศ. <span class="year-info"></span>
  </div>

  <div class="section position">
    <h3 class="title">จำแนกตามตำแหน่งที่นั่ง</h3>
    <div class="w-50 -light">
      <img src="http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/bicycle-all.png" srcset="http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/bicycle-all.png 1x, http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/bicycle-all@2x.png 2x" alt="รวมผู้ขับขี่และผู้โดยสาร" class="bicycle" />
      <div class="bubble -large all">
        <span class="label">รวมผู้ขับขี่<br>และผู้โดยสาร</span>
        <div class="value"></div>
      </div>
    </div>
    <div class="w-25">
      <img src="http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/bicycle-driver.png" srcset="http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/bicycle-driver.png 1x, http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/bicycle-driver@2x.png 2x" alt="เฉพาะผู้ขับขี่" class="bicycle"/>
      <div class="bubble -small driver">
        <span class="label">เฉพาะ<br>ผู้ขับขี่</span>
        <div class="value"></div>
      </div>
    </div>
    <div class="w-25 -light">
      <img src="http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/bicycle-passenger.png" srcset="http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/bicycle-passenger.png 1x, http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/bicycle-passenger@2x.png 2x" alt="เฉพาะผู้โดยสาร" class="bicycle" />
      <div class="bubble -small passenger">
        <span class="label">เฉพาะ<br>ผู้โดยสาร</span>
        <div class="value"></div>
      </div>
    </div>
  </div>

  <div class="section age">
    <h3 class="title">จำแนกตามกลุ่มอายุ</h3>
    <div class="block-age">
      <div class="section-left">
        <div class="adult">
          <div class="block-percent -adult">
            <div class="value circle"></div>
            <span class="name">ผู้ใหญ่</span>
          </div>
          <span class="label">รวมผู้ขับขี่และผู้โดยสาร</span>
          <img src="http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/bicycle-adult.png" srcset="http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/bicycle-adult.png 1x, http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/bicycle-adult@2x.png 2x" alt="เด็กและผู้ใหญ่" class="bicycle-adult" />
        </div>
        <div class="child">
          <div class="block-percent -child">
            <span class="name">&nbsp;เด็ก</span>
            <div class="value circle"></div>
          </div>
          <span class="label">*ในกลุ่มเด็กพิจารณาเฉพาะผู้โดยสาร</span>
        </div>
      </div>
      <div class="section-right">
        <div class="teen">
          <div class="block-percent -teen">
            <span class="name">วัยรุ่น</span>
            <div class="value circle"></div>
          </div>
          <span class="label">รวมผู้ขับขี่และผู้โดยสาร</span>
          <img src="http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/bicycle-teen.png" srcset="http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/bicycle-teen.png 1x, http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/bicycle-teen@2x.png 2x" alt="วัยรุ่น" class="bicycle-teen" />
        </div>
      </div>
    </div>
  </div>

  <div class="section area">
    <h3 class="title">จำแนกตามพื้นที่</h3>
    <div class="block-area">
      <table class="table-area">
       <tr>
         <th></th><th>ชุมชนเมืองหลัก</th><th>ชุมชนเมืองรอง</th><th>ชุมชนชนบท</th><th>รวม</th>
       </tr>
       <tr class="bd-bt all">
         <td class="title"><img src="http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/area-all.png" srcset="http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/area-all.png 1x, http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/area-all@2x.png 2x" alt="รวมผู้ขับขี่และผู้โดยสาร" />
           <br />รวมผู้ขับขี่และผู้โดยสาร</td>
         <td><div class="value circle"></div></td>
         <td><div class="value circle"></div></td>
         <td><div class="value circle"></div></td>
         <td><div class="value summary"></div></td>
       </tr>
       <tr class="bd-bt driver">
         <td class="title"><img src="http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/area-driver.png" srcset="http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/area-driver.png 1x, http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/area-driver@2x.png 2x" alt="เฉพาะผู้ขับขี่" />
           <br />เฉพาะผู้ขับขี่</td>
         <td><div class="value circle"></div></td>
         <td><div class="value circle"></div></td>
         <td><div class="value circle"></div></td>
         <td><div class="value summary"></div></td>
       </tr>
       <tr class="passenger">
         <td class="title"><img src="http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/area-passenger.png" srcset="http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/area-passenger.png 1x, http://trso-helmet.thairoads.org/sites/all/modules/thairoads/helmet/img/area-passenger@2x.png 2x" alt="เฉพาะผู้โดยสาร" />
           <br />เฉพาะผู้โดยสาร</td>
         <td><div class="value circle"></div></td>
         <td><div class="value circle"></div></td>
         <td><div class="value circle"></div></td>
         <td><div class="value summary"></div></td>
       </tr>
      </table>
    </div>
    <div class="remark">
      <h3 class="title">หมายเหตุ</h3>
      <strong>ชุมชนเมืองหลัก</strong> หมายถึง เขตเทศบาลนครหรือเทศบาลเมืองที่เป็นศูนย์กลางกิจกรรมทางเศรษฐกิจของจังหวัด
      <br />
      <strong>ชุมชนเมืองรอง</strong> หมายถึง เขตเทศบาลเมืองในอําเภออื่นๆ หรือเทศบาลตําบลที่มีประชากรมากกว่า 20,000 คน
      <br />
      <strong>ชุมชนชนบท</strong> หมายถึง เทศบาลตําบลที่มีประชากรน้อยกว่า 20,000 คน
    </div>
  </div>

</div>
